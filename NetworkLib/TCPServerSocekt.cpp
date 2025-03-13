#include "pch.h"
#include "TCPServerSocket.h"
#include "IPacket.h"
#include "PrePacketDef.h"   
#include "Serializer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <cstring>
#include <chrono>

using namespace std::chrono;
namespace Net {

    TCPServerSocket::TCPServerSocket()
        : iocpHandle_(NULL), running_(false),
        numWorkerThreads_(std::thread::hardware_concurrency()),
        buffer_size_(4096),
        heartbeatIntervalMs_(5000),
        heartbeatTimeoutMs_(15000)
    {
    }

    TCPServerSocket::~TCPServerSocket()
    {
        Stop();
    }

    bool TCPServerSocket::Start(const NetAddress& address, int thread_count)
    {
        numWorkerThreads_ = thread_count > 0 ? thread_count : std::thread::hardware_concurrency();

        if (!listenSocket_.Create()) {
            std::cerr << "리슨 소켓 생성 실패" << std::endl;
            return false;
        }

        if (!listenSocket_.Bind(address, true)) {
            std::cerr << "바인드 실패" << std::endl;
            return false;
        }

        if (!listenSocket_.Listen(SOMAXCONN)) {
            std::cerr << "Listen 실패" << std::endl;
            return false;
        }

        iocpHandle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, numWorkerThreads_);
        if (iocpHandle_ == NULL) {
            std::cerr << "CreateIoCompletionPort 실패" << std::endl;
            return false;
        }

        running_.store(true);

        for (int i = 0; i < numWorkerThreads_; ++i) {
            workerThreads_.emplace_back(&TCPServerSocket::WorkerThread, this);
        }

        acceptThread_ = std::thread(&TCPServerSocket::AcceptThread, this);
        heartbeatThread_ = std::thread(&TCPServerSocket::HeartbeatThread, this);

        std::cout << "IOCP 서버 시작: " << address.ip_address << ":" << address.port << std::endl;
        return true;
    }

    void TCPServerSocket::Stop()
    {
        running_.store(false);
        listenSocket_.Close();

        for (int i = 0; i < numWorkerThreads_; ++i) {
            PostQueuedCompletionStatus(iocpHandle_, 0, 0, nullptr);
        }

        for (auto& t : workerThreads_) {
            if (t.joinable())
                t.join();
        }
        workerThreads_.clear();

        if (acceptThread_.joinable())
            acceptThread_.join();
        if (heartbeatThread_.joinable())
            heartbeatThread_.join();

        if (iocpHandle_ != NULL)
            CloseHandle(iocpHandle_);
    }

    size_t TCPServerSocket::GetClientCount()
    {
        return connectionManager_.GetClientCount();
    }

    bool TCPServerSocket::RegisterClient(SOCKET clientSocket)
    {
        if (CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle_, (ULONG_PTR)clientSocket, 0) == NULL) {
            std::cerr << "클라이언트 소켓 IOCP 등록 실패" << std::endl;
            return false;
        }
        return true;
    }

    bool TCPServerSocket::StartRecv(SOCKET clientSocket)
    {
        TCPIOContext* pContext = new TCPIOContext();
        pContext->clientSocket = clientSocket;
        pContext->wsabuf.buf = new char[buffer_size_];
        pContext->wsabuf.len = buffer_size_;
        pContext->recvAccumulated.clear();

        DWORD flags = 0;

        int ret = WSARecv(clientSocket, &pContext->wsabuf, 1, NULL, &flags, &pContext->overlapped, NULL);
        if (ret == SOCKET_ERROR) 
        {
            int err = WSAGetLastError();
            if (err != WSA_IO_PENDING) 
            {
                std::cerr << "초기 WSARecv 실패: " << err << std::endl;
                closesocket(clientSocket);
                delete[] pContext->wsabuf.buf;
                delete pContext;
                return false;
            }
        }
        return true;
    }

    void TCPServerSocket::AcceptThread()
    {
        while (running_.load()) 
        {
            NetTCPSocket clientSocket;
            if (!listenSocket_.Accept(clientSocket)) 
            {
                if (!running_.load()) break;
                std::cerr << "클라이언트 Accept 실패: " << WSAGetLastError() << std::endl;
                continue;
            }

            SOCKET s = clientSocket.GetInternalSocket();
            NetAddress addr;
            if (!GetClientAddress(s, addr))
            {
                addr = NetAddress("Unknown", 0);
            }
            connectionManager_.AddClient(s, addr);
            TCPConnectionState state = connectionManager_.GetClientState(s);
            if (OnClientAccepted)
                OnClientAccepted(state);
            std::cout << "클라이언트 접속, 소켓: " << s << ", UniqueKey: " << state.uniqueKey << std::endl;

            if (!RegisterClient(s))
                continue;
            if (!StartRecv(s))
                continue;
        }
    }

    bool TCPServerSocket::ProcessAccumulatedData(TCPIOContext* pContext, std::vector<char>& packetData)
    {
        if (pContext->recvAccumulated.size() < sizeof(uint32_t))
        {
            return false;
        }

        // 패킷 총 길이
         uint32_t netLength = 0;
        memcpy(&netLength, pContext->recvAccumulated.data(), sizeof(uint32_t));

        uint32_t packetLength = ntohl(netLength);
        if (pContext->recvAccumulated.size() < sizeof(uint32_t) + packetLength)
        {
            // 하나의 패킷을 전부 받지 못함
            return false;
        }

        // 패킷 데이터만 분리하여 packetData로 반환
        packetData.assign(pContext->recvAccumulated.begin() + sizeof(uint32_t),
            pContext->recvAccumulated.begin() + sizeof(uint32_t) + packetLength);

        pContext->recvAccumulated.erase(pContext->recvAccumulated.begin(),
            pContext->recvAccumulated.begin() + sizeof(uint32_t) + packetLength);

        return true;
    }

    bool TCPServerSocket::GetClientAddress(SOCKET clientSocket, NetAddress& address)
    {
        sockaddr_in addr = {};
        int addrLen = sizeof(addr);
        if (getpeername(clientSocket, (sockaddr*)&addr, &addrLen) == SOCKET_ERROR)
            return false;

        char ipBuffer[INET_ADDRSTRLEN] = { 0 };
        if (inet_ntop(AF_INET, &addr.sin_addr, ipBuffer, sizeof(ipBuffer)) == nullptr)
            return false;

        address = NetAddress(std::string(ipBuffer), ntohs(addr.sin_port));
        return true;
    }

    void TCPServerSocket::WorkerThread()
    {
        DWORD bytesTransferred = 0;
        ULONG_PTR completionKey = 0;
        TCPIOContext* pContext = nullptr;

        while (running_.load()) 
        {
            BOOL result = GetQueuedCompletionStatus(iocpHandle_, &bytesTransferred, &completionKey, (LPOVERLAPPED*)&pContext, INFINITE);
            if (!result) 
            {
                std::cerr << "GetQueuedCompletionStatus 실패: " << GetLastError() << std::endl;
                continue;
            }

            if (bytesTransferred == 0 || pContext == nullptr) 
            {
                std::cout << "클라이언트 종료, 소켓: " << completionKey << std::endl;
                CloseClient((SOCKET)completionKey);
                continue;
            }

            if (pContext->recvAccumulated.empty()) 
            {
                pContext->recvAccumulated.assign(pContext->wsabuf.buf, pContext->wsabuf.buf + bytesTransferred);
            }
            else 
            {
                pContext->recvAccumulated.insert(pContext->recvAccumulated.end(),
                    pContext->wsabuf.buf,
                    pContext->wsabuf.buf + bytesTransferred);
            }

            std::vector<char> packetData;
            while (ProcessAccumulatedData(pContext, packetData)) 
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
                    std::cerr << "패킷 생성 실패, packet_id: " << payloadHeader.packet_id << std::endl;
                    continue;
                }

                std::vector<BYTE> payload(packetData.begin() + sizeof(PayloadHeader), packetData.end());
                Serializer serializer(payload);
                packet->Deserialize(serializer);

                NetAddress clientAddr;
                if (!GetClientAddress((SOCKET)completionKey, clientAddr))
                {
                    clientAddr = NetAddress("Unknown", 0);
                }
                TCPConnectionState state = connectionManager_.GetClientState((SOCKET)completionKey);

                // 응답 시간 갱신
                if (packet->GetPacketID() == NET_PACKET_ID_PONG)
                {
                    connectionManager_.UpdateClientResponseTime((SOCKET)completionKey);
                }
                else
                {
                    if (OnPacketReceived)
                        OnPacketReceived(state, std::move(packet));
                }
            }

            // 다시 Recv IOCP 예약
            DWORD flags = 0;
            ZeroMemory(&pContext->overlapped, sizeof(OVERLAPPED));
            pContext->wsabuf.len = buffer_size_;
            int ret = WSARecv((SOCKET)completionKey, &pContext->wsabuf, 1, NULL, &flags, &pContext->overlapped, NULL);
            if (ret == SOCKET_ERROR) 
            {
                int err = WSAGetLastError();
                if (err != WSA_IO_PENDING) 
                {
                    std::cerr << "WSARecv 재호출 실패: " << err << std::endl;
                    CloseClient((SOCKET)completionKey);
                    delete[] pContext->wsabuf.buf;
                    delete pContext;
                }
            }
        }
    }

    void TCPServerSocket::CloseClient(SOCKET clientSocket)
    {
        NetAddress addr;
        GetClientAddress(clientSocket, addr);

        TCPConnectionState state = connectionManager_.GetClientState(clientSocket);
        connectionManager_.RemoveClient(clientSocket);
        closesocket(clientSocket);
        if (OnClientClosed)
        {
            OnClientClosed(state);
        }
    }

    bool TCPServerSocket::SendPacketToClient(long long uniqueKey, const IPacket& packet)
    {
        SOCKET targetSocket = 0;
        {
            for (auto& pair : connectionManager_.GetAllConnections()) 
            {
                if (pair.second.uniqueKey == uniqueKey)
                {
                    targetSocket = pair.first;
                    break;
                }
            }
        }
        if (targetSocket == 0) 
        {
            std::cerr << "SendPacketToClient: 대상 클라이언트를 찾을 수 없습니다." << std::endl;
            return false;
        }
        // Payload( 데이터 )
        Serializer serializer;
        packet.Serialize(serializer);
        std::vector<BYTE> payload = serializer.GetData();

        // Payload Header ( 패킷 정보 )
        PayloadHeader header;
        header.packet_id = packet.GetPacketID();

        // Payload Header + Payload;
        std::vector<BYTE> packetData;
        BYTE* headerPtr = reinterpret_cast<BYTE*>(&header);
        packetData.insert(packetData.end(), headerPtr, headerPtr + sizeof(PayloadHeader));
        packetData.insert(packetData.end(), payload.begin(), payload.end());

        // 패킷 총 길이 + Payload Header + Payload
        uint32_t len = static_cast<uint32_t>(packetData.size());
        uint32_t netLen = htonl(len);
        std::vector<BYTE> sendBuffer;
        BYTE* lenPtr = reinterpret_cast<BYTE*>(&netLen);
        sendBuffer.insert(sendBuffer.end(), lenPtr, lenPtr + sizeof(uint32_t));
        sendBuffer.insert(sendBuffer.end(), packetData.begin(), packetData.end());

        // 전송
        int ret = send(targetSocket, reinterpret_cast<const char*>(sendBuffer.data()), static_cast<int>(sendBuffer.size()), 0);
        if (ret == SOCKET_ERROR) 
        {
            std::cerr << "SendPacketToClient 전송 실패: " << WSAGetLastError() << std::endl;

			int err = WSAGetLastError();
            if (err == WSAECONNRESET || err == WSAECONNABORTED) 
            {
                CloseClient(targetSocket);
            }
            return false;
        }
        return true;
    }

    void TCPServerSocket::HeartbeatThread()
    {
        while (running_.load()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(heartbeatIntervalMs_));
            std::vector<SOCKET> toClose;
            auto allConnections = connectionManager_.GetAllConnections();
            for (auto& pair : allConnections) 
            {
                auto now = steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pair.second.lastResponseTime).count();
                if (duration > heartbeatTimeoutMs_) 
                {
                    std::cerr << "하트비트 타임아웃: 클라이언트 고유키 " << pair.second.uniqueKey << std::endl;
                    toClose.push_back(pair.first);
                }
                else 
                {
                    // 하트비트로 PingPacket 전송
                    PingPacket ping;
                    SendPacketToClient(pair.second.uniqueKey, ping);
                }
            }
            for (SOCKET s : toClose) {
                CloseClient(s);
            }
        }
    }

} // namespace Net
