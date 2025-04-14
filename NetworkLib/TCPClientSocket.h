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

        // IPacket을 보낸 후 응답 콜백. 콜백 등록할 때 this가 사라지는 것 주의
        bool SendAndReceivePacket(IPacket& request_packet, uint32_t timeout_ms, std::function<void(std::unique_ptr<IPacket>)> callback);

		// 수신된 패킷 처리. client_key은 무조건 0으로 넘겨줌
        void ProcessPacketsFromQueue(std::function<void(ReceivedPacketInfo&)> callback);

        // Serializer 팩토리 함수
        void SetSerializerFactory(std::function<std::unique_ptr<Serializer>()> factory) { serializer_factory_ = factory; }

    private:
        NetTCPSocket socket_;
        std::vector<char> recv_buffer_; // 누적 수신 데이터 버퍼
        int buffer_size_;
        std::atomic<bool> running_;
        std::thread recv_thread_;

        std::atomic<uint32_t> next_pending_number_;                             // 응답을 기다리려는 패킷에 부여할 다음 번호
        std::unordered_map<uint32_t, PendingPacketCallback> pending_packet_;    // 패킷의 응답이 올 때 호출될 콜백 저장
        std::mutex pending_packets_mutex_;                                      // pending_packets_에 대한 동기화

        // Serialize 팩토리
        std::function<std::unique_ptr<Serializer>()> serializer_factory_;

        // 내부 함수: 수신 스레드 루프
        void RecvThread();

        // 내부 함수: 누적 버퍼에서 완전한 패킷 추출 ([4바이트 길이][PayloadHeader][payload])
        bool ProcessBuffer(std::vector<char>& packet_data);

        // 수신된 패킷을 저장하는 큐
        ConcurrentQueue<ReceivedPacketInfo> recv_data_queue_;
    };
} // namespace Net
