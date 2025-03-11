#pragma once
#include "NetDef.h"
#include <map>
#include <mutex>
#include <atomic>
#include <chrono>

namespace Net {

    struct TCPConnectionState {
        long long uniqueKey;  // 고유 키
        NetAddress address;   // IP
        SOCKET socket;        // 소켓
        std::chrono::steady_clock::time_point lastResponseTime; // 마지막 응답 시간
    };

    class TCPConnectionManager {
    public:
        TCPConnectionManager();

        // 클라이언트 추가: 클라이언트 소켓과 주소를 받아 고유키를 부여하고 저장합니다.
        void AddClient(SOCKET socket, const NetAddress& address);

        // 클라이언트 제거
        void RemoveClient(SOCKET socket);

        // 소켓으로 클라이언트 상태 조회
        TCPConnectionState GetClientState(SOCKET socket);

        // 모든 클라이언트의 상태를 반환
        std::map<SOCKET, TCPConnectionState> GetAllConnections();

		// 클라이언트 응답 시간 업데이트
        void UpdateClientResponseTime(SOCKET socket);

        // 접속 중인 클라이언트 수를 반환
        size_t GetClientCount();

    private:
        std::map<SOCKET, TCPConnectionState> connections_;
        std::mutex mutex_;
        std::atomic<long long> nextUniqueKey_;
    };

} // namespace Net
