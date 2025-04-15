// UDPPacketSocket.h
#pragma once
#include "NetUDPSocket.h"
#include "NetDef.h"
#include <chrono>
#include <mutex>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <random>
#include <functional>
#include <atomic>
#include <thread>
#include "ConcurrentQueue.h"
#include "IPacket.h"
#include "UDPConnectionManager.h"

namespace Net::UDP{

#pragma pack(push, 1)
    struct PacketHeader {
        uint32_t sequence_number; // 데이터 시퀀스 번호
        uint32_t ack_number;      // ACK 번호
        uint8_t flags;            // PacketFlags
        uint16_t data_length;     // payload 길이
        uint32_t checksum;        // 패킷 체크섬
    };
#pragma pack(pop)

    // 패킷 플래그
    enum PacketFlags {
        FLAG_DATA = 0,
        FLAG_R_DATA = 1,
        FLAG_ACK = 2,
        FLAG_CONNECT = 3, // 신규 연결 요청용 플래그
    };

    struct ReceivedPacketInfo
    {
        NetAddress address;
        std::unique_ptr<IPacket> packet;
    };

    class UDPPacketSocket {
    public:
        UDPPacketSocket();
        virtual ~UDPPacketSocket();

#ifdef DEBUG_LOSS_TEST
        bool SendLossTest(const NetAddress& net_address, const BYTE* data, int data_length, int& sent_length);
#endif

        // 패킷 전송
        bool SendPacket(const NetAddress& net_address, const IPacket& packet, bool reliable);

        // 수신된 패킷 처리 후 반환
        std::pair<bool, bool> RecvPacket(ReceivedPacketInfo& packetInfo);

        // 지금까지 수신된 패킷 큐에 저장 ( RecvPacket 래퍼 )
        void ReceivePacketsToQueue();

        // 큐에 저장된 패킷을 콜백처리
        void ProcessPacketsFromQueue(const std::function<void(ReceivedPacketInfo&)>& callback);

        // 전송 실패, ACK 받지 못한 패킷 재전송
        void ProcessTimeouts();

        bool Create() {
            return net_socket_.Create(); 
        }
        bool Bind(const NetAddress& net_address, bool reuse_addr) {
            return net_socket_.Bind(net_address, reuse_addr); 
        }
        void Close() {
            net_socket_.Close(); 
        }
        UDPConnectionManager& GetConnectionManager() {
            return connection_manager_;
        }

        void SetOnConnectCallback(const std::function<void(const NetAddress&)>& callback) {
            connection_manager_.SetOnConnectCallback(callback);
        }
        void SetOnDisconnectCallback(const std::function<void(const NetAddress&)>& callback) {
            connection_manager_.SetOnDisconnectCallback(callback);
        }

        void SetBufferSize(int size);
        inline int GetBufferSize() { return buffer_size_; }

    private:
        bool SendPacketInternal(UDPConnectionState& conn, const IPacket& packet, bool reliable);
        void SendAck(const NetAddress& net_address, uint32_t sequence_number);
        uint32_t CalculateChecksum(const BYTE* data, int length);
        uint32_t CalculateChecksum(const PacketHeader& header, const BYTE* payload, int payload_length);
        bool SendUnacknowledgedPacket(UDPConnectionState& conn, uint32_t sequence_number);

        int window_size_;
        int buffer_size_;
#ifdef DEBUG_LOSS_TEST
        double simulated_loss_rate_{ 0.3 };
        std::mt19937 rng_{ std::random_device{}() };
        std::uniform_real_distribution<double> dist_{ 0.0, 1.0 };
#endif

        ConcurrentQueue<ReceivedPacketInfo> recv_data_queue_;
        UDPConnectionManager connection_manager_;
        NetUDPSocket net_socket_;
    };

} // namespace Net
