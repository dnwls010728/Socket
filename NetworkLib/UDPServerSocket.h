// UDPServerSocket.h
#pragma once
#include "UDPPacketSocket.h"
#include "IPacket.h"
#include "PrePacketDef.h"
#include "Serializer.h"
#include <functional>
#include <atomic>
#include <thread>
#include <vector>

namespace Net::UDP {

    class UDPServerSocket {
    public:
        UDPServerSocket();
        virtual ~UDPServerSocket();

        bool Start(const NetAddress& bindAddress);
        void Stop();

        // 클라이언트에게 패킷 전송 (신뢰성 옵션 기본 true)
        bool SendPacket(const NetAddress& clientAddress, const IPacket& packet, bool reliable = true);

        // 사용자가 직접 수신된 패킷을 처리하도록 호출하는 함수
        void ProcessPacketsFromQueue(const std::function<void(ReceivedPacketInfo&)>& callback);

        // 하트비트 스레드 관련 설정
        void SetHeartbeatInterval(int ms) { heartbeatIntervalMs_ = ms; }
        void SetHeartbeatTimeout(int ms) { heartbeatTimeoutMs_ = ms; }

        void SetOnConnectCallback(const std::function<void(const NetAddress&)>& callback) {
            udpSocket_.SetOnConnectCallback(callback);
        }
        void SetOnDisconnectCallback(const std::function<void(const NetAddress&)>& callback) {
            udpSocket_.SetOnDisconnectCallback(callback);
        }

    private:
        void RecvThread();
        void HeartbeatThread();

        UDPPacketSocket udpSocket_;
        std::atomic<bool> running_;
        std::thread recvThread_;
        std::thread heartbeatThread_;

        int heartbeatIntervalMs_;  // 하트비트 전송 간격
        int heartbeatTimeoutMs_;   // 타임아웃 기준 시간
    };

} // namespace Net
