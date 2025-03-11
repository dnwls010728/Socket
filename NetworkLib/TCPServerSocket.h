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

namespace Net {

    // IOCP용 컨텍스트 구조체
    struct TCPIOContext 
    {
        OVERLAPPED overlapped;   // I/O 완료 통지용 OVERLAPPED 구조체
        WSABUF wsabuf;           // 수신 버퍼용 WSABUF
        SOCKET clientSocket;     // 클라이언트 소켓 (NetTCPSocket에서 받아온 내부 핸들)
        std::vector<char> recvAccumulated;  // 누적 수신 데이터를 보관하여 패킷 경계를 맞추기 위한 버퍼
    };

    class TCPServerSocket {
    public:
        TCPServerSocket();
        ~TCPServerSocket();

        // 서버 소켓 생성, 바인드, Listen. thread_count = 0이면 CPU 코어 수만큼 스레드 생성
        bool Start(const NetAddress& address, int thread_count = 0);

        // 서버 종료 및 리소스 해제
        void Stop();

        // 콜백 인터페이스: 모든 콜백은 TCPConnectionState를 전달합니다.
        std::function<void(const TCPConnectionState&, std::unique_ptr<IPacket> packet)> OnPacketReceived;
        std::function<void(const TCPConnectionState&)> OnClientAccepted;
        std::function<void(const TCPConnectionState&)> OnClientClosed;

        // 송/수신 버퍼 크기 설정
        void SetBufferSize(int size) { buffer_size_ = size; }

        // 하트비트 설정: 주기(ms)와 타임아웃(ms)
        void SetHeartbeatParameters(int intervalMs, int timeoutMs) { heartbeatIntervalMs_ = intervalMs; heartbeatTimeoutMs_ = timeoutMs; }

        // 접속 중인 클라이언트 수
        size_t GetClientCount();

        // 특정 고유키를 가진 클라이언트에게 IPacket 전송
        bool SendPacketToClient(long long uniqueKey, const IPacket& packet);

    private:
        NetTCPSocket listenSocket_;
        HANDLE iocpHandle_;
        std::vector<std::thread> workerThreads_;
        std::atomic<bool> running_;
        int numWorkerThreads_;
        int buffer_size_;

        std::thread acceptThread_;
        std::thread heartbeatThread_;

        // 하트비트 주기 및 타임아웃 (밀리초)
        int heartbeatIntervalMs_;
        int heartbeatTimeoutMs_;

        // TCPConnectionManager를 사용하여 클라이언트 정보를 관리
        TCPConnectionManager connectionManager_;

        // IOCP 워커 스레드 함수
        void WorkerThread();

        // Accept 스레드 함수: 클라이언트 연결 수락 후 IOCP 등록, 초기 Recv 시작, 클라이언트 정보 저장
        void AcceptThread();

        // 하트비트 스레드 함수: 주기적으로 PingPacket 전송 및 타임아웃 확인
        void HeartbeatThread();

        // 클라이언트 소켓에 대해 초기 WSARecv 호출
        bool StartRecv(SOCKET clientSocket);

        // 클라이언트 소켓을 IOCP에 등록
        bool RegisterClient(SOCKET clientSocket);

        // 내부 함수: 누적 데이터(recvAccumulated)에서 완전한 패킷 추출 ([4바이트 길이][PayloadHeader][payload])
        bool ProcessAccumulatedData(TCPIOContext* pContext, std::vector<char>& packetData);

        // 내부 함수: clientSocket의 IP, Port 정보 획득
        bool GetClientAddress(SOCKET clientSocket, NetAddress& address);

        // 내부 함수: 클라이언트 종료 처리 (소켓 닫고, TCPConnectionManager에서 제거 및 콜백 호출)
        void CloseClient(SOCKET clientSocket);
    };

} // namespace Net
