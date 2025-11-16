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
#include <queue>
#include <mutex>

namespace Net::TCP {

    TCPServerSocket::TCPServerSocket()
        : iocp_handle_(NULL), running_(false),
        num_worker_threads_(std::thread::hardware_concurrency()),
        buffer_size_(4096),
        heartbeat_Interval_ms_(5000),
        heartbeat_timeout_ms_(15000),
		OnClientAccepted(nullptr),
		OnClientClosed(nullptr),
		OnPacketReceived(nullptr)
    {
    }

    TCPServerSocket::~TCPServerSocket()
    {
        Stop();
    }

    bool TCPServerSocket::Start(const NetAddress& address, int thread_count)
    {
        if (running_.load() == true)
            return true;

        num_worker_threads_ = thread_count > 0 ? thread_count : std::thread::hardware_concurrency();

        if (!listen_socket_.Create()) {
            std::cerr << "리슨 소켓 생성 실패" << std::endl;
            return false;
        }

        if (!listen_socket_.Bind(address, true)) {
            std::cerr << "바인드 실패" << std::endl;
            return false;
        }

        if (!listen_socket_.Listen(SOMAXCONN)) {
            std::cerr << "Listen 실패" << std::endl;
            return false;
        }

        iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, num_worker_threads_);
        if (iocp_handle_ == NULL) {
            std::cerr << "CreateIoCompletionPort 실패" << std::endl;
            return false;
        }

        running_.store(true);

        for (int i = 0; i < num_worker_threads_; ++i) {
            worker_threads_.emplace_back(&TCPServerSocket::WorkerThread, this);
        }

        accept_thread_ = std::thread(&TCPServerSocket::AcceptThread, this);
        heartbeat_thread_ = std::thread(&TCPServerSocket::HeartbeatThread, this);

        std::cout << "IOCP 서버 시작: " << address.ip_address << ":" << address.port << std::endl;
        return true;
    }

    void TCPServerSocket::Stop()
    {
        if (running_.load() == false)
        {
            return;
        }
        running_.store(false);
        listen_socket_.Close();

        for (int i = 0; i < num_worker_threads_; ++i) {
            PostQueuedCompletionStatus(iocp_handle_, 0, 0, nullptr);
        }

        for (auto& t : worker_threads_) {
            if (t.joinable())
                t.join();
        }
        worker_threads_.clear();

        if (accept_thread_.joinable())
            accept_thread_.join();
        if (heartbeat_thread_.joinable())
            heartbeat_thread_.join();

        if (iocp_handle_ != NULL)
        {
            CloseHandle(iocp_handle_);
            iocp_handle_ = NULL;
        }
    }

    size_t TCPServerSocket::GetClientCount()
    {
        return connection_manager_.GetClientCount();
    }

    bool TCPServerSocket::RegisterClient(SOCKET client_socket)
    {
        if (CreateIoCompletionPort((HANDLE)client_socket, iocp_handle_, (ULONG_PTR)client_socket, 0) == NULL) {
            std::cerr << "클라이언트 소켓 IOCP 등록 실패" << std::endl;
            return false;
        }
        return true;
    }
    

    bool TCPServerSocket::StartRecv(SOCKET client_socket)
    {
        TCPIOContext* p_context = new TCPIOContext();
        p_context->clientSocket = client_socket;
        p_context->wsabuf.buf = new char[buffer_size_];
        p_context->wsabuf.len = buffer_size_;
        p_context->recvAccumulated.clear();

        DWORD flags = 0;

        int ret = WSARecv(client_socket, &p_context->wsabuf, 1, NULL, &flags, &p_context->overlapped, NULL);
        if (ret == SOCKET_ERROR)
        {
            int err = WSAGetLastError();
            if (err != WSA_IO_PENDING)
            {
                std::cerr << "초기 WSARecv 실패: " << err << std::endl;
                closesocket(client_socket);
                delete[] p_context->wsabuf.buf;
                delete p_context;
                return false;
            }
        }
        return true;
    }

    void TCPServerSocket::AcceptThread()
    {
        while (running_.load())
        {
            NetTCPSocket client_socket;
            if (!listen_socket_.Accept(client_socket))
            {
                if (!running_.load()) break;
                std::cerr << "클라이언트 Accept 실패: " << WSAGetLastError() << std::endl;
                continue;
            }

            SOCKET s = client_socket.GetInternalSocket();
            NetAddress addr;
            if (!GetClientAddress(s, addr))
            {
                addr = NetAddress("Unknown", 0);
            }
            uint32_t client_key = connection_manager_.AddClient(s, addr);
            TCPConnectionState state = connection_manager_.GetClientState(client_key);
            if (OnClientAccepted)
            {
                bool accept = OnClientAccepted(state);
                if (accept == false)
                {
                    connection_manager_.RemoveClient(client_key);
                    continue;
                }
            }
            std::cout << "클라이언트 접속, 키: " << client_key << ", UniqueKey: " << state.uniqueKey << std::endl;

            if (!RegisterClient(s))
                continue;
            if (!StartRecv(s))
                continue;
        }
    }

    bool TCPServerSocket::ProcessAccumulatedData(TCPIOContext* p_context, std::vector<char>& packet_data)
    {
        if (p_context->recvAccumulated.size() < sizeof(uint32_t))
        {
            return false;
        }

        uint32_t net_length = 0;
        memcpy(&net_length, p_context->recvAccumulated.data(), sizeof(uint32_t));

        uint32_t packet_length = ntohl(net_length);
        if (p_context->recvAccumulated.size() < sizeof(uint32_t) + packet_length)
        {
            return false;
        }

        packet_data.assign(p_context->recvAccumulated.begin() + sizeof(uint32_t),
            p_context->recvAccumulated.begin() + sizeof(uint32_t) + packet_length);

        p_context->recvAccumulated.erase(p_context->recvAccumulated.begin(),
            p_context->recvAccumulated.begin() + sizeof(uint32_t) + packet_length);

        return true;
    }

    bool TCPServerSocket::GetClientAddress(SOCKET client_socket, NetAddress& address)
    {
        sockaddr_in addr = {};
        int addrLen = sizeof(addr);
        if (getpeername(client_socket, (sockaddr*)&addr, &addrLen) == SOCKET_ERROR)
            return false;

        char ip_buffer[INET_ADDRSTRLEN] = { 0 };
        if (inet_ntop(AF_INET, &addr.sin_addr, ip_buffer, sizeof(ip_buffer)) == nullptr)
            return false;

        address = NetAddress(std::string(ip_buffer), ntohs(addr.sin_port));
        return true;
    }

    void TCPServerSocket::EnqueueDisconnect(uint32_t unique_key)
    {
        {
            std::lock_guard<std::mutex> lock(disconnect_mutex_);
            disconnect_queue_.push(unique_key);
        }
        PostQueuedCompletionStatus(iocp_handle_, 0, 0, nullptr);
    }

    void TCPServerSocket::ProcessDisconnectQueue()
    {
        std::queue<uint32_t> local;
        {
            std::lock_guard<std::mutex> lock(disconnect_mutex_);
            std::swap(local, disconnect_queue_);
        }

        while (!local.empty())
        {
            uint32_t key = local.front();
            local.pop();
            DisconnectClientInternal(key);
        }
    }

    void TCPServerSocket::WorkerThread()
    {
        DWORD bytes_transferred = 0;
        ULONG_PTR completion_key = 0;
        TCPIOContext* p_context = nullptr;

        while (running_.load())
        {
            BOOL result = GetQueuedCompletionStatus(iocp_handle_, &bytes_transferred, &completion_key, (LPOVERLAPPED*)&p_context, INFINITE);

            ProcessDisconnectQueue();

            if (!result)
            {
                if (p_context)
                {
                    delete[] p_context->wsabuf.buf;
                    delete p_context;
                    p_context = nullptr;
                }

                if (completion_key != NULL)
                {
                    TCPConnectionState state = connection_manager_.GetClientStateBySocket((SOCKET)completion_key);
                    if (state.uniqueKey != 0)
                    {
                        EnqueueDisconnect(state.uniqueKey);
                    }
                }
                continue;
            }

            if (bytes_transferred == 0 || p_context == nullptr)
            {
                if (p_context)
                {
                    delete[] p_context->wsabuf.buf;
                    delete p_context;
                    p_context = nullptr;
                }

                if (completion_key != NULL)
                {
                    std::cout << "Client disconnected, socket: " << completion_key << std::endl;
                    TCPConnectionState state = connection_manager_.GetClientStateBySocket((SOCKET)completion_key);
                    if (state.uniqueKey != 0)
                    {
                        EnqueueDisconnect(state.uniqueKey);
                    }
                }
                continue;
            }

            if (p_context->recvAccumulated.empty())
            {
                p_context->recvAccumulated.assign(p_context->wsabuf.buf, p_context->wsabuf.buf + bytes_transferred);
            }
            else
            {
                p_context->recvAccumulated.insert(p_context->recvAccumulated.end(),
                    p_context->wsabuf.buf,
                    p_context->wsabuf.buf + bytes_transferred);
            }

            std::vector<char> packet_data;
            while (ProcessAccumulatedData(p_context, packet_data))
            {
                if (packet_data.size() < sizeof(PayloadHeader))
                {
                    continue;
                }
                PayloadHeader payloadHeader;
                memcpy(&payloadHeader, packet_data.data(), sizeof(PayloadHeader));
                std::unique_ptr<IPacket> packet = PacketFactoryRegistry::Instance().CreatePacket(payloadHeader.packet_id);
                if (!packet)
                {
                    std::cerr << "create packet failed, packet_id: " << payloadHeader.packet_id << std::endl;
                    continue;
                }

                std::vector<BYTE> payload(packet_data.begin() + sizeof(PayloadHeader), packet_data.end());

                Serializer serializer;
                serializer.SetData(payload);
                packet->Deserialize(serializer);

                NetAddress client_addr;
                if (!GetClientAddress((SOCKET)completion_key, client_addr))
                {
                    client_addr = NetAddress("Unknown", 0);
                }

                TCPConnectionState state = connection_manager_.GetClientStateBySocket((SOCKET)completion_key);
                connection_manager_.UpdateClientResponseTime(state.uniqueKey);
				//recv_data_queue_.push({ state.uniqueKey, std::move(packet) });

                if (packet->GetPacketID() == NET_PACKET_ID_PING_REQUEST)
                {
                    PingRequestPacket* ping_request_packet = static_cast<PingRequestPacket*>(packet.get());
                    
                    PingPacket ping_packet;
                    ping_packet.client_time = ping_request_packet->client_time;
                    ping_packet.server_time = Net::GetClientTime();
                    SendPacketToClient(state.uniqueKey, ping_packet);
                }

                if (OnPacketReceived)
                {
					OnPacketReceived(state, std::move(packet));
                }

            }

            DWORD flags = 0;
            ZeroMemory(&p_context->overlapped, sizeof(OVERLAPPED));
            p_context->wsabuf.len = buffer_size_;
            int ret = WSARecv((SOCKET)completion_key, &p_context->wsabuf, 1, NULL, &flags, &p_context->overlapped, NULL);
            if (ret == SOCKET_ERROR)
            {
                int err = WSAGetLastError();
                if (err != WSA_IO_PENDING)
                {
                    std::cerr << "WSARecv failed : " << err << std::endl;
                    TCPConnectionState state = connection_manager_.GetClientStateBySocket((SOCKET)completion_key);
                    if (state.uniqueKey != 0)
                    {
                        EnqueueDisconnect(state.uniqueKey);
                    }
                    delete[] p_context->wsabuf.buf;
                    delete p_context;
                    p_context = nullptr;
                }
            }
        }
    }

    bool TCPServerSocket::DisconnectClientInternal(uint32_t unique_key)
    {
        TCPConnectionState state = connection_manager_.GetClientState(unique_key);
        if (state.uniqueKey == 0)
        {
            return false;
        }
        connection_manager_.RemoveClient(unique_key);
        closesocket(state.socket);
        if (OnClientClosed)
        {
            OnClientClosed(state);
        }
        return true;
    }

    bool TCPServerSocket::DisconnectClient(uint32_t unique_key)
    {
        EnqueueDisconnect(unique_key);
        return true;
    }

    bool TCPServerSocket::SendPacketToClient(uint32_t unique_key, const IPacket& kPacket)
    {
        SOCKET target_socket = INVALID_SOCKET;
        TCPConnectionState client_state = connection_manager_.GetClientState(unique_key);
        if (client_state.uniqueKey == 0)
        {
            std::cerr << "not found client, unique_key: " << unique_key << std::endl;
            return false;
        }
        target_socket = client_state.socket;

        Serializer serializer;
        kPacket.Serialize(serializer);
        std::vector<BYTE> payload = serializer.GetData();

        PayloadHeader header;
        header.packet_id = kPacket.GetPacketID();
        std::vector<BYTE> packet_data;
        BYTE* header_ptr = reinterpret_cast<BYTE*>(&header);
        packet_data.insert(packet_data.end(), header_ptr, header_ptr + sizeof(PayloadHeader));
        packet_data.insert(packet_data.end(), payload.begin(), payload.end());
        
        uint32_t len = static_cast<uint32_t>(packet_data.size());
        uint32_t netLen = htonl(len);
        std::vector<BYTE> send_buffer;
        BYTE* len_ptr = reinterpret_cast<BYTE*>(&netLen);
        send_buffer.insert(send_buffer.end(), len_ptr, len_ptr + sizeof(uint32_t));
        send_buffer.insert(send_buffer.end(), packet_data.begin(), packet_data.end());
        int ret = send(target_socket, reinterpret_cast<const char*>(send_buffer.data()), static_cast<int>(send_buffer.size()), 0);
        if (ret == SOCKET_ERROR)
        {
            std::cerr << "SendPacketToClient send failed : " << WSAGetLastError() << std::endl;

            int err = WSAGetLastError();
            if (err == WSAECONNRESET || err == WSAECONNABORTED)
            {
                DisconnectClient(unique_key);
            }
            return false;
        }
        return true;
    }

    void TCPServerSocket::HeartbeatThread()
    {
        while (running_.load())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(heartbeat_Interval_ms_));
            std::vector<uint32_t> to_close;
            auto allConnections = connection_manager_.GetAllConnections();
            for (auto& pair : allConnections)
            {
                auto now = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pair.second.lastResponseTime).count();
                if (duration > heartbeat_timeout_ms_)
                {
                    std::cerr << "Heartbeat TimeOut, uniqueKey : " << pair.second.uniqueKey << std::endl;
                    to_close.push_back(pair.first);
                }
            }
            for (uint32_t unique_key : to_close) {
                DisconnectClient(unique_key);
            }
        }
    }

} // namespace Net
