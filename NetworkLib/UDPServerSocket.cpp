// UDPServerSocket.cpp
#include "pch.h"
#include "UDPServerSocket.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono;

namespace Net {

    UDPServerSocket::UDPServerSocket()
        : running_(false),
        heartbeatIntervalMs_(5000),
        heartbeatTimeoutMs_(15000)
    {
    }

    UDPServerSocket::~UDPServerSocket()
    {
        Stop();
    }

    bool UDPServerSocket::Start(const NetAddress& bindAddress)
    {
        if (!udpSocket_.Create())
        {
            std::cerr << "UDPServerSocket: 소켓 생성 실패" << std::endl;
            return false;
        }
        if (!udpSocket_.Bind(bindAddress, true))
        {
            std::cerr << "UDPServerSocket: 바인드 실패" << std::endl;
            return false;
        }

        running_.store(true);
        recvThread_ = std::thread(&UDPServerSocket::RecvThread, this);
        heartbeatThread_ = std::thread(&UDPServerSocket::HeartbeatThread, this);

        return true;
    }

    void UDPServerSocket::Stop()
    {
        running_.store(false);

        if (recvThread_.joinable())
            recvThread_.join();
        if (heartbeatThread_.joinable())
            heartbeatThread_.join();
    }

    bool UDPServerSocket::SendPacket(const NetAddress& clientAddress, const IPacket& packet, bool reliable)
    {
        return udpSocket_.SendPacket(clientAddress, packet, reliable);
    }

    void UDPServerSocket::ProcessPacketsFromQueue(const std::function<void(RecvPacketInfo&)>& callback)
    {
        udpSocket_.ProcessPacketsFromQueue([this, callback](RecvPacketInfo& recvInfo) {
            if (recvInfo.packet_id == NET_PACKET_ID_PONG)
            {
                udpSocket_.GetConnectionManager().UpdateClientResponseTime(recvInfo.address);
            }
            else
            {
                if (callback) callback(recvInfo);
            }
            });
    }

    // 내부 수신 스레드
    void UDPServerSocket::RecvThread()
    {
        while (running_.load())
        {
            udpSocket_.ReceivePacketsToQueue();
            udpSocket_.ProcessTimeouts();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    // 하트비트 스레드: 주기적으로 모든 클라이언트에 PingPacket 전송, 타임아웃된 클라이언트는 연결 제거
    void UDPServerSocket::HeartbeatThread()
    {
        while (running_.load())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(heartbeatIntervalMs_));
            std::vector<NetAddress> toRemove;
            std::vector<NetAddress> toPing;

            // ForEachConnection 내에서 필요한 정보만 복사
            udpSocket_.GetConnectionManager().ForEachConnection([&](UDPConnectionState& state) {
                auto now = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - state.last_activity_time).count();
                if (duration > heartbeatTimeoutMs_)
                {
                    toRemove.push_back(state.remote_address);
                }
                else
                {
                    toPing.push_back(state.remote_address);
                }
                });

            // 핑 전송
            for (const auto& addr : toPing)
            {
                SendPacket(addr, PingPacket(), true);
            }

            // 타임아웃된 클라이언트 제거
            for (const auto& addr : toRemove)
            {
                udpSocket_.GetConnectionManager().RemoveClient(addr);
            }
        }
    }

} // namespace Net
