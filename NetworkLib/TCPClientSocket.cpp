#include "pch.h"
#include "TCPClientSocket.h"
#include "IPacket.h"
#include "PrePacketDef.h"
#include "Serializer.h"
#include <iostream>
#include <cstring>

namespace Net {

    TCPClientSocket::TCPClientSocket()
        : bufferSize_(8192), running_(false)
    {
        recvBuffer_.reserve(bufferSize_);
    }

    TCPClientSocket::~TCPClientSocket()
    {
        Stop();
    }

    bool TCPClientSocket::Start(const NetAddress& serverAddress)
    {
        if (!socket_.Create()) 
        {
            std::cerr << "TCPClientSocket: 소켓 생성 실패" << std::endl;
            return false;
        }
        if (!socket_.Connect(serverAddress))
        {
            std::cerr << "TCPClientSocket: 서버 연결 실패" << std::endl;
            return false;
        }
        running_.store(true);
        recvThread_ = std::thread(&TCPClientSocket::RecvThread, this);
        return true;
    }

    void TCPClientSocket::Stop()
    {
        running_.store(false);
        socket_.Close();
        if (recvThread_.joinable())
        {
            recvThread_.join();
        }
    }

    bool TCPClientSocket::SendPacket(const IPacket& packet)
    {
        // 1. IPacket 직렬화
        Serializer serializer;
        packet.Serialize(serializer);
        std::vector<BYTE> payload = serializer.GetData();

        // 2. PayloadHeader 작성
        PayloadHeader header;
        header.packet_id = packet.GetPacketID();

        // 3. [PayloadHeader][payload] 결합
        std::vector<BYTE> packetData;
        BYTE* headerPtr = reinterpret_cast<BYTE*>(&header);
        packetData.insert(packetData.end(), headerPtr, headerPtr + sizeof(PayloadHeader));
        packetData.insert(packetData.end(), payload.begin(), payload.end());

		// 4. 전체 패킷 길이 + [PayloadHeader][payload] 조합
        uint32_t len = static_cast<uint32_t>(packetData.size());
        uint32_t netLen = htonl(len);
        std::vector<BYTE> sendBuffer;
        BYTE* lenPtr = reinterpret_cast<BYTE*>(&netLen);
        sendBuffer.insert(sendBuffer.end(), lenPtr, lenPtr + sizeof(uint32_t));
        sendBuffer.insert(sendBuffer.end(), packetData.begin(), packetData.end());

        // 전송
        int sentLength = 0;
        if (!socket_.Send(reinterpret_cast<const char*>(sendBuffer.data()),
            static_cast<int>(sendBuffer.size()), sentLength)) 
        {
            std::cerr << "TCPClientSocket: SendPacket 전송 실패, 에러: " << WSAGetLastError() << std::endl;
            return false;
        }
        return true;
    }

    bool TCPClientSocket::ProcessBuffer(std::vector<char>& packetData)
    {
        if (recvBuffer_.size() < sizeof(uint32_t))
            return false;

        uint32_t netLength = 0;
        memcpy(&netLength, recvBuffer_.data(), sizeof(uint32_t));
        uint32_t packetLength = ntohl(netLength);

        // 하나의 패킷을 전부 받지 못함
        if (recvBuffer_.size() < sizeof(uint32_t) + packetLength)
        {
            return false;
        }

        // 패킷을 추출하여 packetData로 반환
        packetData.assign(recvBuffer_.begin() + sizeof(uint32_t),
            recvBuffer_.begin() + sizeof(uint32_t) + packetLength);
        recvBuffer_.erase(recvBuffer_.begin(), recvBuffer_.begin() + sizeof(uint32_t) + packetLength);

        return true;
    }

    void TCPClientSocket::RecvThread()
    {
        while (running_.load()) {
            char tempBuffer[1024] = { 0 };
            int received = 0;
            if (!socket_.Recv(tempBuffer, sizeof(tempBuffer), received)) 
            {
                if (received <= 0) 
                {
                    std::cerr << "TCPClientSocket: Recv 실패 또는 연결 종료" << std::endl;
                    running_.store(false);
                    break;
                }
            }
            // 누적 버퍼에 수신된 데이터를 추가
            recvBuffer_.insert(recvBuffer_.end(), tempBuffer, tempBuffer + received);
            std::vector<char> packetData;
            // 누적 데이터에서 완전한 패킷이 있으면 처리
            while (ProcessBuffer(packetData)) 
            {
                if (packetData.size() < sizeof(PayloadHeader)) 
                {
                    continue;
                }
                PayloadHeader payloadHeader;
                memcpy(&payloadHeader, packetData.data(), sizeof(PayloadHeader));
                std::unique_ptr<IPacket> packet = PacketFactoryRegistry::Instance().CreatePacket(payloadHeader.packet_id);
                if (!packet) 
                {
                    std::cerr << "TCPClientSocket: 패킷 생성 실패, packet_id: " << payloadHeader.packet_id << std::endl;
                    continue;
                }

                // 역릭렬화
                std::vector<BYTE> payload(packetData.begin() + sizeof(PayloadHeader), packetData.end());
                Serializer serializer(payload);
                packet->Deserialize(serializer);

				if (packet->GetPacketID() == NET_PACKET_ID_PING)
				{
					PongPacket pongPacket;
					SendPacket(pongPacket);
				}
                else
                {
                    if (OnPacketReceived)
                    {
                        OnPacketReceived(std::move(packet));
                    }
                }
            }
        }
    }
    
} // namespace Net
