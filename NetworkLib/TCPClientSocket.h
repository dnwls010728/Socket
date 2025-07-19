#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <memory>
#include "NetTCPSocket.h"
#include "NetDef.h"
#include "IPacket.h"
#include "ConcurrentQueue.h"

namespace Net::TCP {

    class TCPClientSocket {
    public:
        TCPClientSocket();
        ~TCPClientSocket();

        // Start: 소켓 생성, 서버 연결 및 수신 스레드 시작
        bool Start(const NetAddress& serverAddress);

        // Stop: 연결 종료 및 수신 스레드 정리
        void Stop();

        // IPacket 전송 (패킷 형식: [4바이트 길이][PayloadHeader][payload])
        bool SendPacket(IPacket& packet);

		// 수신된 패킷 처리. client_key은 무조건 0으로 넘겨줌
        void ProcessPacketsFromQueue(std::function<void(ReceivedPacketInfo&)> callback);

        // 서버 연결 끊김 콜백
        void SetDisconnectedCallback(std::function<void()> callback) { OnDisconnected = callback; }

        inline void SetPingRequestPeriod(int ms) { ping_request_period_ms_ = ms; }
        inline int GetPingRequestPeriod() const { return ping_request_period_ms_; }
        inline void SetBufferSize(int size) { buffer_size_ = size; }
        inline int GetBufferSize() const{ return buffer_size_; }
        inline float GetServerTime() const { return GetClientTime() + server_time_offset;}
    private:
        // PingRequest 보낸시간과 Ping을 받은 시간으로 서버와 클라이언트의 시간 차이를 계산
        float CalculateServerTimeOffset(float sent_client_time, float recv_server_time, float old_offset);
        bool is_first_ping;
        
        NetTCPSocket socket_;
        std::vector<char> recv_buffer_; // 누적 수신 데이터 버퍼
        int buffer_size_;
        std::atomic<bool> running_;
        std::thread recv_thread_;
        int ping_request_period_ms_;
        float last_request_time;
        float server_time_offset;

        // 내부 함수: 수신 스레드 루프
        void RecvThread();
        void SendPingRequest(float now);

        // 내부 함수: 누적 버퍼에서 완전한 패킷 추출 ([4바이트 길이][PayloadHeader][payload])
        bool ProcessBuffer(std::vector<char>& packet_data);

        // 수신된 패킷을 저장하는 큐
        ConcurrentQueue<ReceivedPacketInfo> recv_data_queue_;

        // 서버 연결 끊김 콜백
        std::function<void()> OnDisconnected;
    };
} // namespace Net
