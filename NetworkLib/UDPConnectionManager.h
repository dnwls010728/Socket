#pragma once
#include <chrono>
#include <mutex>
#include <vector>
#include <map>
#include <string>
#include "Windows.h"
#include "NetDef.h"
#include <queue>

namespace Net {

    // 전송 한 패킷을 관리하기 위한 구조체
    // 패킷 전송 후 RetransmittablePacket 상태로 해당 패킷을 보관
    // 대상이 sequence_number에 맞는 ACK를 보내면 삭제, 받지 못했다고 판단되면 재전송
    struct RetransmittablePacket
    {
        uint32_t sequence_number;                           // 데이터 시퀀스 번호       
        std::chrono::steady_clock::time_point send_time;	// 전송 시간
        std::vector<BYTE> data;							    // 전송한 데이터 
        int retransmission_count;                           // 재전송 횟수
    };

    struct UDPConnectionState
    {
        // 수신 관련
        uint32_t expected_sequence_number;                          // 다음에 받아야 할 시퀀스 번호
        std::map<uint32_t, std::vector<BYTE>> reorder_buffer;         // 수신하였지만 순서에 맞지 않을 경우 reorder_buffer에 저장
        std::map<uint32_t, int> duplicate_ack_counts;                 // 중복 ACK 카운트. 일정 수 이상 받으면 빠른 재전송

        // 전송 관련
        uint32_t next_sequence_number;                                // 다음 전송 시퀀스 번호
        std::map<uint32_t, RetransmittablePacket> unacknowledged_packets;   // 전송한 패킷 목록
        std::queue<std::pair<uint32_t, std::vector<BYTE>>> pending_packets_; // 전송 실패한 패킷 목록

        // 연결 관리
        std::chrono::steady_clock::time_point last_activity_time;   // 마지막 응답 시간
        NetAddress remote_address;
        std::mutex state_mutex;                                     // ConnectionState 락
    };

    class UDPConnectionManager
    {
    public:
        // 클라이언트 상태 반환
        UDPConnectionState* GetConnectionState(const NetAddress& net_address);

        UDPConnectionState& AddClient(const NetAddress& net_address);

		// 클라이언트 삭제
        void RemoveClient(const NetAddress& net_address);

        void SetOnConnectCallback(const std::function<void(const NetAddress&)>& callback) {
            on_connect_callback_ = callback;
        }
        void SetOnDisconnectCallback(const std::function<void(const NetAddress&)>& callback) {
            on_disconnect_callback_ = callback;
        }

		// 모든 클라이언트 반환
        //std::map<std::string, UDPConnectionState> GetAllConnections();

		// 클라이언트 응답 시간 업데이트
        void UpdateClientResponseTime(const NetAddress& net_address);

		// 클라이언트 수 반환   
        size_t GetClientCount();

        template<typename Func>
        void ForEachConnection(Func f) {
            std::lock_guard<std::mutex> lock(connections_mutex_);
            for (auto& pair : connections_) {
                f(pair.second);
            }
        }
    private:
        std::function<void(const NetAddress& net_address)> on_connect_callback_;
        std::function<void(const NetAddress& net_address)> on_disconnect_callback_;

        std::mutex connections_mutex_; // connections_ 락
        std::map<std::string, UDPConnectionState> connections_;
    };
} // namespace Net