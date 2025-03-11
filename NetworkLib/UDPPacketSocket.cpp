// UDPPacketSocket.cpp
#include "pch.h"
#include "UDPPacketSocket.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>

namespace Net {

    uint32_t UDPPacketSocket::CalculateChecksum(const BYTE* data, int length)
    {
        uint32_t sum = 0;
        for (int i = 0; i < length; ++i)
            sum += static_cast<uint8_t>(data[i]);
        return ~sum;
    }

    uint32_t UDPPacketSocket::CalculateChecksum(const PacketHeader& header, const BYTE* payload, int payload_length)
    {
        PacketHeader temp = header;
        temp.checksum = 0;
        int totalSize = sizeof(PacketHeader) + payload_length;
        std::vector<BYTE> buffer(totalSize);
        memcpy(buffer.data(), &temp, sizeof(PacketHeader));
        memcpy(buffer.data() + sizeof(PacketHeader), payload, payload_length);
        return CalculateChecksum(buffer.data(), totalSize);
    }

#ifdef DEBUG_LOSS_TEST
    bool UDPPacketSocket::SendLossTest(const NetAddress& net_address, const BYTE* data, int data_length, int& sent_length)
    {
        if (simulated_loss_rate_ > 0.0 && dist_(rng_) < simulated_loss_rate_)
        {
            sent_length = data_length;
            return true;
        }
        return net_socket_.SendTo(net_address, reinterpret_cast<const char*>(data), data_length, sent_length);
    }
#endif

    UDPPacketSocket::UDPPacketSocket() :
        window_size_(10),
        buffer_size_(2048)
    {
        net_socket_.Create();
    }

    UDPPacketSocket::~UDPPacketSocket()
    {
        Close();
    }

    bool UDPPacketSocket::SendPacket(const NetAddress& net_address, const IPacket& packet, bool reliable)
    {
        UDPConnectionState* pConn = connection_manager_.GetConnectionState(net_address);
        if (pConn == nullptr)
        {
            pConn = &connection_manager_.AddClient(net_address);
        }
        UDPConnectionState& conn = *pConn;

        if (conn.unacknowledged_packets.size() >= static_cast<size_t>(window_size_))
        {
            Serializer serializer;
            packet.Serialize(serializer);
            std::vector<BYTE> payload = serializer.GetData();
            uint32_t seq_num = conn.next_sequence_number;
            conn.pending_packets_.push({ seq_num, payload });
            std::cout << "[SendPacket] 윈도우가 가득 참. 패킷 대기열에 추가됨: " << seq_num << std::endl;

            return false;
        }

        return SendPacketInternal(conn, packet, reliable);
    }

    bool UDPPacketSocket::SendPacketInternal(UDPConnectionState& conn, const IPacket& packet, bool reliable)
    {
        std::vector<BYTE> data;

        // 직렬화
        Serializer serializer;
        packet.Serialize(serializer);
        std::vector<BYTE> payload = serializer.GetData();

        // 버퍼에 헤더 삽입
        PayloadHeader payload_header;
        payload_header.packet_id = packet.GetPacketID();
        BYTE* header_ptr = reinterpret_cast<BYTE*>(&payload_header);
        data.insert(data.begin(), header_ptr, header_ptr + sizeof(PayloadHeader));

        // 버퍼에 직렬화 한 데이터 삽입
        data.insert(data.end(), payload.begin(), payload.end());

        int data_length = static_cast<int>(data.size() + sizeof(PayloadHeader));

        {
            std::lock_guard<std::mutex> lock(conn.state_mutex);

            // 패킷 헤더 생성
            PacketHeader header{ 0, };
            header.data_length = htons(static_cast<uint16_t>(data_length));
            if (reliable)
            {
                header.sequence_number = htonl(conn.next_sequence_number);  // 패킷 번호
                header.ack_number = 0;
                header.flags = FLAG_R_DATA;

                // 데이터를 전송하기 전에 미리 저장
                RetransmittablePacket retrans_pkt;
                retrans_pkt.sequence_number = ntohl(header.sequence_number);
                retrans_pkt.send_time = std::chrono::steady_clock::now();
                retrans_pkt.data = data;
                retrans_pkt.retransmission_count = 0;
                conn.unacknowledged_packets[retrans_pkt.sequence_number] = std::move(retrans_pkt);
            }
            else
            {
                header.flags = FLAG_DATA;
            }
            // 체크섬 계산
            header.checksum = htonl(CalculateChecksum(header, data.data(), data.size()));

            int headerSize = sizeof(PacketHeader);
            int packetSize = headerSize + data_length;
            std::vector<BYTE> packetBuffer(packetSize);
            memcpy(packetBuffer.data(), &header, headerSize);
            memcpy(packetBuffer.data() + headerSize, data.data(), data.size());

            int sent_length = 0;
#ifdef DEBUG_LOSS_TEST
            bool ret = SendLossTest(conn.remote_address, packetBuffer.data(), packetSize, sent_length);
#else
            bool ret = net_socket_.SendTo(conn.remote_address, reinterpret_cast<const char*>(packetBuffer.data()), packetSize, sent_length);
#endif
            // 전송 실패
            if (!ret || sent_length != packetSize)
            {
                std::cerr << "[SendPacketInternal] 전송 실패" << std::endl;
                conn.unacknowledged_packets.erase(conn.next_sequence_number);
                return false;
            }

            if (reliable)
            {
                conn.next_sequence_number++;
            }
        }
        return true;
    }

    std::pair<bool, bool> UDPPacketSocket::RecvPacket(RecvPacketInfo& packetInfo)
    {
        if (buffer_size_ < static_cast<int>(sizeof(PacketHeader)))
        {
            return { true, false };
        }

        NetAddress net_address;
        std::unique_ptr<BYTE[]> buffer(new BYTE[buffer_size_]);
        int buffer_length = 0;
        if (!net_socket_.RecvFrom(reinterpret_cast<char*>(buffer.get()), buffer_size_, buffer_length, net_address))
        {
            return { false, false };
        }

        if (buffer_length < static_cast<int>(sizeof(PacketHeader)))
        {
            return { true, false };
        }

        PacketHeader header;
        memcpy(&header, buffer.get(), sizeof(PacketHeader));
        header.sequence_number = ntohl(header.sequence_number);
        header.ack_number = ntohl(header.ack_number);
        header.data_length = ntohs(header.data_length);
        header.checksum = ntohl(header.checksum);

        int payload_length = header.data_length;
        if (buffer_length < static_cast<int>(sizeof(PacketHeader)) + payload_length)
        {
            return { true, false };
        }

        // 체크섬 계산
        uint32_t calculated_checksum = CalculateChecksum(header, buffer.get() + sizeof(PacketHeader), payload_length);
        if (calculated_checksum != header.checksum)
        {
            return { true, false };
        }

        UDPConnectionState* pConn = connection_manager_.GetConnectionState(net_address);
        if (pConn == nullptr)
        {
			pConn = &(connection_manager_.AddClient(net_address));
        }
        UDPConnectionState& conn = *pConn;
        {
            std::lock_guard<std::mutex> lock(conn.state_mutex);
            conn.last_activity_time = std::chrono::steady_clock::now();

            if (header.flags == FLAG_ACK)
            {
                // 데인터 송신에 대한 응답코드 수신
                if (conn.unacknowledged_packets.find(header.ack_number) != conn.unacknowledged_packets.end())
                {
                    conn.unacknowledged_packets.erase(header.ack_number);
                    conn.duplicate_ack_counts[header.ack_number] = 0;
                }
                else
                {
                    int& duplicate_ack_count = conn.duplicate_ack_counts[header.ack_number];
                    duplicate_ack_count++;
                    if (duplicate_ack_count >= 3)
                    {
                        duplicate_ack_count = 0;
                        SendUnacknowledgedPacket(conn, header.ack_number + 1);
                    }
                }
                return { true, false };
            }
            else if (header.flags == FLAG_R_DATA)
            {
                if (header.sequence_number < conn.expected_sequence_number)
                {
                    // 예상된 패킷보다 이전 패킷 수신
                    SendAck(net_address, header.sequence_number);
                    return { true, false };
                }
                else if (header.sequence_number == conn.expected_sequence_number)
                {
                    // 예상된 패킷 수신
                    SendAck(net_address, header.sequence_number);
                    conn.expected_sequence_number++;

                    PayloadHeader payload_header;
                    BYTE* payload_header_start = buffer.get() + sizeof(PacketHeader);
                    memcpy(&payload_header, payload_header_start, sizeof(PayloadHeader));

                    std::unique_ptr<IPacket> packet = PacketFactoryRegistry::Instance().CreatePacket(payload_header.packet_id);
                    if (packet.get() == nullptr)
                    {
                        return { true, false };
                    }

                    BYTE* payload_start = payload_header_start + sizeof(PayloadHeader);
                    std::vector<BYTE> data(payload_start, payload_start + payload_length - sizeof(PayloadHeader));
                    Serializer serializer(data);
                    packet->Deserialize(serializer);

                    packetInfo.address = conn.remote_address;
                    packetInfo.packet_id = payload_header.packet_id;
                    packetInfo.packet = std::move(packet);

                    // 이전에 먼저온 패킷들 처리
                    while (true)
                    {
                        auto it = conn.reorder_buffer.find(conn.expected_sequence_number);
                        if (it != conn.reorder_buffer.end())
                        {
                            SendAck(net_address, it->first);
                            conn.reorder_buffer.erase(conn.expected_sequence_number);
                            conn.expected_sequence_number++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    return { true, true };
                }
                else if (header.sequence_number > conn.expected_sequence_number)
                {
                    // 예상된 패킷보다 이후 패킷 저장
                    SendAck(net_address, conn.expected_sequence_number - 1);
                    std::vector<BYTE> data;
                    BYTE* payload_start = buffer.get() + sizeof(PacketHeader);
                    data.assign(payload_start, payload_start + payload_length);
                    conn.reorder_buffer[header.sequence_number] = std::move(data);
                    return { true, false };
                }
            }
            else if (header.flags == FLAG_DATA)
            {
                // 비 신뢰 패킷 수신
                PayloadHeader payload_header;
                BYTE* payload_header_start = buffer.get() + sizeof(PacketHeader);
                memcpy(&payload_header, payload_header_start, sizeof(PayloadHeader));

                std::unique_ptr<IPacket> packet = PacketFactoryRegistry::Instance().CreatePacket(payload_header.packet_id);
                if (packet.get() == nullptr)
                {
                    return { true, false };
                }

                BYTE* payload_start = payload_header_start + sizeof(PayloadHeader);
                std::vector<BYTE> data(payload_start, payload_start + payload_length);
                Serializer serializer(data);
                packet->Deserialize(serializer);

                packetInfo.address = conn.remote_address;
                packetInfo.packet_id = payload_header.packet_id;
                packetInfo.packet = std::move(packet);
                return { true, true };
            }
        }
        return { true, false };
    }

    void UDPPacketSocket::ReceivePacketsToQueue()
    {
        while (true)
        {
            RecvPacketInfo packet_Info;
            auto result = RecvPacket(packet_Info);
            if (!result.first)
                break;
            if (result.second)
            {
                recv_data_queue_.push(std::move(packet_Info));
            }
        }
    }

    void UDPPacketSocket::ProcessPacketsFromQueue(const std::function<void(RecvPacketInfo&)>& callback)
    {
        if (!callback) return;

        RecvPacketInfo packet_info;
        while (recv_data_queue_.try_pop(packet_info))
        {
            callback(packet_info);
        }
    }

    void UDPPacketSocket::SendAck(const NetAddress& net_address, uint32_t sequence_number)
    {
        PacketHeader header;
        header.sequence_number = 0;
        header.ack_number = htonl(sequence_number);
        header.flags = FLAG_ACK;
        header.data_length = 0;
        header.checksum = 0;
        header.checksum = htonl(CalculateChecksum(reinterpret_cast<const BYTE*>(&header), sizeof(PacketHeader)));

        int sent_length = 0;
#ifdef DEBUG_LOSS_TEST
        bool ret = SendLossTest(net_address, reinterpret_cast<const BYTE*>(&header), sizeof(PacketHeader), sent_length);
#else
        bool ret = net_socket_.SendTo(net_address, reinterpret_cast<const char*>(&header), sizeof(PacketHeader), sent_length);
#endif
    }

    void UDPPacketSocket::ProcessTimeouts()
    {
        auto now = std::chrono::steady_clock::now();
        connection_manager_.ForEachConnection([this, now](UDPConnectionState& conn) {
            std::lock_guard<std::mutex> lock(conn.state_mutex);
            for (auto it = conn.unacknowledged_packets.begin(); it != conn.unacknowledged_packets.end(); )
            {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second.send_time);
                if (elapsed.count() >= 500)
                {
                    SendUnacknowledgedPacket(conn, it->second.sequence_number);
                }
                ++it;
            }

            while (!conn.pending_packets_.empty())
            {
                auto [seq_num, payload] = conn.pending_packets_.front();
                conn.pending_packets_.pop();

                PacketHeader header{ 0, };
                header.sequence_number = htonl(seq_num);
                header.ack_number = 0;
                header.flags = FLAG_R_DATA;
                header.data_length = htons(static_cast<uint16_t>(payload.size()));
                header.checksum = htonl(CalculateChecksum(header, payload.data(), payload.size()));

                int packetSize = sizeof(PacketHeader) + payload.size();
                std::vector<BYTE> packetBuffer(packetSize);
                memcpy(packetBuffer.data(), &header, sizeof(PacketHeader));
                memcpy(packetBuffer.data() + sizeof(PacketHeader), payload.data(), payload.size());

                int sent_length = 0;
                if (net_socket_.SendTo(conn.remote_address, reinterpret_cast<char*>(packetBuffer.data()), packetSize, sent_length))
                {
                    RetransmittablePacket retrans_pkt;
                    retrans_pkt.sequence_number = seq_num;
                    retrans_pkt.send_time = std::chrono::steady_clock::now();
                    retrans_pkt.data = payload;
                    retrans_pkt.retransmission_count = 0;

                    conn.unacknowledged_packets[seq_num] = std::move(retrans_pkt);
                    conn.next_sequence_number++;
                }
            }
            });
    }

    void UDPPacketSocket::SetBufferSize(int size)
    {
        buffer_size_ = size;
    }

    bool UDPPacketSocket::SendUnacknowledgedPacket(UDPConnectionState& conn, uint32_t sequence_number)
    {
        auto it = conn.unacknowledged_packets.find(sequence_number);
        if (it == conn.unacknowledged_packets.end())
        {
            return false;
        }

        RetransmittablePacket& packet = it->second;
        PacketHeader header;
        header.sequence_number = htonl(packet.sequence_number);
        header.ack_number = 0;
        header.flags = FLAG_R_DATA;
        header.data_length = htons(static_cast<uint16_t>(packet.data.size()));
        header.checksum = 0;
        header.checksum = htonl(CalculateChecksum(header, packet.data.data(), packet.data.size()));

        int headerSize = sizeof(PacketHeader);
        int packetSize = headerSize + packet.data.size();
        std::vector<BYTE> packetBuffer(packetSize);
        memcpy(packetBuffer.data(), &header, headerSize);
        memcpy(packetBuffer.data() + headerSize, packet.data.data(), packet.data.size());

        int sent_length = 0;
#ifdef DEBUG_LOSS_TEST
        bool ret = SendLossTest(conn.remote_address, packetBuffer.data(), packetSize, sent_length);
#else
        bool ret = net_socket_.SendTo(conn.remote_address, reinterpret_cast<const char*>(packetBuffer.data()), packetSize, sent_length);
#endif
        if (ret && sent_length == packetSize)
        {
            it->second.send_time = std::chrono::steady_clock::now();
            it->second.retransmission_count++;
        }
        return ret;
    }


} // namespace Net
