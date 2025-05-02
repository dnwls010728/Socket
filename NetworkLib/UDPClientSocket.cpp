// UDPClientSocket.cpp
#include "pch.h"
#include "UDPClientSocket.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace Net::UDP {

    UDPClientSocket::UDPClientSocket()
        : running_(false)
    {
    }

    UDPClientSocket::~UDPClientSocket()
    {
        Stop();
        udpSocket_.Close();
    }

    bool UDPClientSocket::Start(const NetAddress& serverAddress)
    {
        serverAddress_ = serverAddress;
        if (!udpSocket_.Create())
        {
            std::cerr << "UDPClientSocket: 소켓 생성 실패" << std::endl;
            return false;
        }

        running_.store(true);

        recvThread_ = std::thread(&UDPClientSocket::RecvThread, this);
        return true;
    }

    void UDPClientSocket::Stop()
    {
        running_.store(false);
        if (recvThread_.joinable())
        {
            recvThread_.join();
        }
        udpSocket_.Close();
    }

    bool UDPClientSocket::SendPacket(const IPacket& packet, bool reliable)
    {
        return udpSocket_.SendPacket(serverAddress_, packet, reliable);
    }

    void UDPClientSocket::ProcessPacketsFromQueue(const std::function<void(const ReceivedPacketInfo&)>& callback)
    {
        udpSocket_.ProcessPacketsFromQueue([this, callback](ReceivedPacketInfo& recvInfo) {
            if (recvInfo.packet->GetPacketID() == NET_PACKET_ID_PING)
            {
                udpSocket_.GetConnectionManager().UpdateClientResponseTime(recvInfo.address);
				SendPacket(PongPacket());
            }
            else
            {
                if (callback) callback(recvInfo);
            }
            });
    }

    void UDPClientSocket::RecvThread()
    {
        while (running_.load())
        {
            udpSocket_.ReceivePacketsToQueue();
            udpSocket_.ProcessTimeouts();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

} // namespace Net
