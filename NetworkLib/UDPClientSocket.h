// UDPClientSocket.h
#pragma once
#include "UDPPacketSocket.h"
#include "IPacket.h"
#include "PrePacketDef.h"
#include "Serializer.h"
#include <functional>
#include <atomic>
#include <thread>
#include <vector>

namespace Net {

    class UDPClientSocket {
    public:
        UDPClientSocket();
        virtual ~UDPClientSocket();

        bool Start(const NetAddress& serverAddress);
        void Stop();
        bool SendPacket(const IPacket& packet, bool reliable = true);
        
        // 사용자가 직접 수신된 패킷을 처리하도록 호출하는 함수
        void ProcessPacketsFromQueue(const std::function<void(const RecvPacketInfo&)>& callback);

    private:
        // 내부 수신 처리를 위한 스레드 (필요에 따라 사용)
        void RecvThread();

        UDPPacketSocket udpSocket_;
        NetAddress serverAddress_;
        std::atomic<bool> running_;
        std::thread recvThread_;
    };

} // namespace Net
