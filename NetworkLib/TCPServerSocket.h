#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <functional>
#include "NetTCPSocket.h"
#include "IPacket.h"
#include "NetDef.h"
#include "TCPConnectionManager.h" 
#include "ConcurrentQueue.h"

namespace Net::TCP {

    // IOCP용 컨텍스트 구조체
    struct TCPIOContext 
    {
        OVERLAPPED overlapped;   // I/O 완료 통지용 OVERLAPPED 구조체
        WSABUF wsabuf;           // 수신 버퍼용 WSABUF
        SOCKET clientSocket;     // 클라이언트 소켓 (NetTCPSocket에서 받아온 내부 핸들)
        std::vector<char> recvAccumulated;  // 누적 수신 데이터를 보관하여 패킷 경계를 맞추기 위한 버퍼
    };

    class TCPServerSocket
    {
    public:
    	TCPServerSocket();
    	~TCPServerSocket();

    	// 서버 소켓 생성, 바인드, Listen. thread_count = 0이면 CPU 코어 수만큼 스레드 생성
    	bool Start(const NetAddress& address, int thread_count = 0);

    	// 서버 종료 및 리소스 해제
    	void Stop();

    	void SetClientAcceptedCallback(std::function<bool(const TCPConnectionState&)> callback) { OnClientAccepted = callback; }
    	void SetClientDisconnectedCallback(std::function<void(const TCPConnectionState&)> callback) { OnClientClosed = callback; }
    	void SetPacketReceivedCallback(std::function<void(const TCPConnectionState&, std::unique_ptr<IPacket>)> callback) { OnPacketReceived = callback; }

    	// 송/수신 버퍼 크기 설정
    	void SetBufferSize(int size) { buffer_size_ = size; }

    	// 하트비트 설정: 주기(ms)와 타임아웃(ms)
    	void SetHeartbeatParameters(int interval_ms, int timeout_ms) { heartbeat_Interval_ms_ = interval_ms; heartbeat_timeout_ms_ = timeout_ms; }

    	// 접속 중인 클라이언트 수
    	size_t GetClientCount();

    	// 특정 고유키를 가진 클라이언트에게 IPacket 전송
    	bool SendPacketToClient(uint32_t unique_key, const IPacket& kPacket);

    	// IPacket을 보낸 후 응답 콜백
    	bool SendAndReceivePacket(uint32_t unique_key, IPacket& request_packet, uint32_t timeout_ms, std::function<void(uint32_t, std::unique_ptr<IPacket>)> callback);

    	// 수신된 패킷 처리
    	// void ProcessPacketsFromQueue(std::function<void(ReceivedPacketInfo&)> callback);

    	// 수신된 패킷을 처리하는 콜백. true를 리턴하면 패킷이 처리됨
    	bool ProcessPendingPacket(TCPConnectionState client_state, std::unique_ptr<IPacket> &packet);

    	// Serializer 팩토리 함수. 실패시 ErrorPacket 수신
    	void SetSerializerFactory(std::function<std::unique_ptr<Serializer>()> factory) { serializer_factory_ = factory; }

    	// 클라이언트 연결 끊기
    	bool DisconnectClient(uint32_t unique_key);
    private:
    	NetTCPSocket listen_socket_;
    	HANDLE iocp_handle_;
    	std::vector<std::thread> worker_threads_;
    	std::atomic<bool> running_;
    	int num_worker_threads_;
    	int buffer_size_;

    	std::thread accept_thread_;
    	std::thread heartbeat_thread_;

    	// 하트비트 주기 및 타임아웃 (밀리초)
    	int heartbeat_Interval_ms_;
    	int heartbeat_timeout_ms_;

    	// 수신된 패킷을 저장하는 큐
    	//ConcurrentQueue<ReceivedPacketInfo> recv_data_queue_;

    	// 콜백
    	std::function<void(const TCPConnectionState&, std::unique_ptr<IPacket> packet)> OnPacketReceived;
    	std::function<bool(const TCPConnectionState&)> OnClientAccepted;
    	std::function<void(const TCPConnectionState&)> OnClientClosed;

    	// Serialize 팩토리
    	std::function<std::unique_ptr<Serializer>()> serializer_factory_;

    	// TCPConnectionManager를 사용하여 클라이언트 정보를 관리
    	TCPConnectionManager connection_manager_;

    	std::atomic<uint32_t> next_pending_number_;                             // 응답을 기다리려는 패킷에 부여할 다음 번호
    	std::unordered_map<uint32_t, PendingPacketCallback> pending_packet_;    // 패킷의 응답이 올 때 호출될 콜백 저장
    	std::mutex pending_packets_mutex_;                                     // pending_packets_에 대한 동기화

    	// IOCP 워커 스레드 함수
    	void WorkerThread();

    	// Accept 스레드 함수: 클라이언트 연결 수락 후 IOCP 등록, 초기 Recv 시작, 클라이언트 정보 저장
    	void AcceptThread();

    	// 하트비트 스레드 함수: 주기적으로 PingPacket 전송 및 타임아웃 확인
    	void HeartbeatThread();

    	// 클라이언트 소켓에 대해 초기 WSARecv 호출
    	bool StartRecv(SOCKET client_socket);

    	// 클라이언트 소켓을 IOCP에 등록
    	bool RegisterClient(SOCKET client_socket);

    	// 내부 함수: 누적 데이터(recvAccumulated)에서 완전한 패킷 추출 ([4바이트 길이][PayloadHeader][payload])
    	bool ProcessAccumulatedData(TCPIOContext* context, std::vector<char>& packet_data);

    	// 내부 함수: clientSocket의 IP, Port 정보 획득
    	bool GetClientAddress(SOCKET client_socket, NetAddress& address);
    };

} // namespace Net
