#include "pch.h"
#include "TCPClientSocket.h"
#include "IPacket.h"
#include "PrePacketDef.h"
#include "Serializer.h"
#include <iostream>
#include <cstring>
#include <chrono> 

namespace Net::TCP {

    TCPClientSocket::TCPClientSocket()
        : buffer_size_(8192), running_(false)
    {
        recv_buffer_.reserve(buffer_size_);
    }

    TCPClientSocket::~TCPClientSocket()
    {
        Stop();
    }

    bool TCPClientSocket::Start(const NetAddress& server_address)
    {
        if (running_.load() == true)
            return true;

        if (!socket_.Create()) 
        {
            std::cerr << "TCPClientSocket: socket create failed" << WSAGetLastError() << std::endl;
            return false;
        }
        if (!socket_.Connect(server_address))
        {
            std::cerr << "TCPClientSocket: server connect failed : "<< WSAGetLastError() << std::endl;
            return false;
        }
        running_.store(true);
        recv_thread_ = std::thread(&TCPClientSocket::RecvThread, this);
        return true;
    }

    void TCPClientSocket::Stop()
    {
        running_.store(false);
        if (recv_thread_.joinable())
        {
            recv_thread_.join();
        }
        socket_.Close();
    }

    bool TCPClientSocket::SendPacket(IPacket& packet)
    {
        // 1. IPacket 직렬화
        Serializer serializer;
        packet.Serialize(serializer);
        std::vector<BYTE> payload = serializer.GetData();

        // 2. PayloadHeader 작성
        PayloadHeader header;
        header.packet_id = packet.GetPacketID();

        // 3. [PayloadHeader][payload] 결합
        std::vector<BYTE> packet_data;
        BYTE* header_ptr = reinterpret_cast<BYTE*>(&header);
        packet_data.insert(packet_data.end(), header_ptr, header_ptr + sizeof(PayloadHeader));
        packet_data.insert(packet_data.end(), payload.begin(), payload.end());

		// 4. 전체 패킷 길이 + [PayloadHeader][payload] 조합
        uint32_t len = static_cast<uint32_t>(packet_data.size());
        uint32_t net_len = htonl(len);
        std::vector<BYTE> send_buffer;
        BYTE* lenPtr = reinterpret_cast<BYTE*>(&net_len);
        send_buffer.insert(send_buffer.end(), lenPtr, lenPtr + sizeof(uint32_t));
        send_buffer.insert(send_buffer.end(), packet_data.begin(), packet_data.end());

        // 전송
        int sent_length = 0;
        if (!socket_.Send(reinterpret_cast<const char*>(send_buffer.data()),
            static_cast<int>(send_buffer.size()), sent_length)) 
        {
            std::cerr << "TCPClientSocket: SendPacket error : " << WSAGetLastError() << std::endl;
            return false;
        }
        return true;
    }

    void TCPClientSocket::ProcessPacketsFromQueue(std::function<void(ReceivedPacketInfo&)> callback)
    {
        if (callback == nullptr)
        {
            return;
        }

        // 쌓여있는 패킷 처리
        ReceivedPacketInfo packet_info;
        while (recv_data_queue_.try_pop(packet_info))
        {
            if (packet_info.packet->GetPacketID() == NET_PACKET_ID_PING)
            {
                PongPacket pong_packet;
                SendPacket(pong_packet);
            }

            // 아닌경우 사용자가 설정한 콜백 호출
            callback(packet_info);
        }
    }

    bool TCPClientSocket::ProcessBuffer(std::vector<char>& packet_data)
    {
        if (recv_buffer_.size() < sizeof(uint32_t))
            return false;

        uint32_t net_length = 0;
        memcpy(&net_length, recv_buffer_.data(), sizeof(uint32_t));
        uint32_t packet_length = ntohl(net_length);

        // 하나의 패킷을 전부 받지 못함
        if (recv_buffer_.size() < sizeof(uint32_t) + packet_length)
        {
            return false;
        }

        // 패킷을 추출하여 packetData로 반환
        packet_data.assign(recv_buffer_.begin() + sizeof(uint32_t),
            recv_buffer_.begin() + sizeof(uint32_t) + packet_length);
        recv_buffer_.erase(recv_buffer_.begin(), recv_buffer_.begin() + sizeof(uint32_t) + packet_length);

        return true;
    }

    void TCPClientSocket::RecvThread()
    {
        while (running_.load()) {
            char temp_buffer[1024] = { 0 };
            int received = 0;
            if (!socket_.Recv(temp_buffer, sizeof(temp_buffer), received)) 
            {
                // 에러 처리
                int err = WSAGetLastError();

                // 논블록
                if (err == WSAEWOULDBLOCK) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                // 연결 끊김
                if (received == 0 || err == WSAECONNRESET || err == WSAECONNABORTED) {
                    std::cerr << "TCPClientSocket: Server disconnected, WSA error=" << err << std::endl;
                    running_.store(false);
                    if (OnDisconnected) OnDisconnected();
                    break;
                }

                // 그 외
                std::cerr << "TCPClientSocket: Recv err, WSA error=" << err << std::endl;
                continue;
            }
            
            // 누적 버퍼에 수신된 데이터를 추가
            recv_buffer_.insert(recv_buffer_.end(), temp_buffer, temp_buffer + received);
            std::vector<char> packet_data;
            // 누적 데이터에서 완전한 패킷이 있으면 처리
            while (ProcessBuffer(packet_data)) 
            {
                if (packet_data.size() < sizeof(PayloadHeader)) 
                {
                    continue;
                }
                PayloadHeader payload_header;
                memcpy(&payload_header, packet_data.data(), sizeof(PayloadHeader));
                std::unique_ptr<IPacket> packet = PacketFactoryRegistry::Instance().CreatePacket(payload_header.packet_id);
                if (!packet) 
                {
                    std::cerr << "TCPClientSocket: Packet create failed, packet_id: " << payload_header.packet_id << std::endl;
                    continue;
                }

                // 역릭렬화
                std::vector<BYTE> payload(packet_data.begin() + sizeof(PayloadHeader), packet_data.end());
                
				Serializer serializer;
				serializer.SetData(payload);
                packet->Deserialize(serializer);

				recv_data_queue_.push({ 0, std::move(packet) });
            }
        }
    }
} // namespace Net
