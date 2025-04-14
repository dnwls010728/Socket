#pragma once
#include <string>
#include <winsock2.h>
#include "NetDef.h"

/*
UDP Server : client.Create(SocketType::kUDPType)
             server.Bind
             server.RecvFrom(&clientAddr)
             server.SendTo(clientAddr)
             server.Close

UDP Client : client.Create(SocketType::kUDPType)
             client.SendTo(serverAddr)
             client.RecvFrom(&serverAddr)
             client.Close
*/

namespace Net::UDP {

    class NetUDPSocket
    {
    public:

        NetUDPSocket();
        ~NetUDPSocket();

        /**
         * @brief 소켓을 생성합니다.
         *
         * @return 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Create();

        /**
         * @brief 소켓을 닫습니다.
         *
         * 소켓 사용 종료 후 반드시 호출해야 합니다.
         */
        void Close();

        /**
         * @brief 소켓을 논블록 모드로 설정합니다.
         *
         * @param is_nonblock 논블록 모드로 설정할 경우 true, 그렇지 않으면 false.
         * @return 설정 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool SetNonBlockingMode(bool is_nonblock);

        /**
         * @brief 소켓을 IP 주소와 포트에 바인딩합니다.
         *
         * 로컬 바인딩을 위해서는 IP: 127.0.0.1을, 모든 사용 가능한 IP에 바인딩하려면 IP: 0.0.0.0을 사용합니다.
         *
         * @param net_address 바인딩할 IP 주소와 포트.
         * @param reuse_addr true일 경우 IP 및 포트 재사용을 허용하여 소켓 종료 후 빠른 재시작이 가능합니다.
         * @return 바인딩 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Bind(const NetAddress& net_address, bool reuse_addr);

        /**
         * @brief 지정된 네트워크 주소로 데이터를 전송합니다.
         *
         * @param net_address 데이터를 전송할 대상 네트워크 주소.
         * @param data 전송할 데이터 버퍼.
         * @param data_length 전송할 데이터의 길이.
         * @param sent_length 실제 전송된 바이트 수를 저장할 변수.
         * @return 전송 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool SendTo(const NetAddress& net_address, const char* data, int data_length, int& sent_length);

        /**
         * @brief 데이터를 수신하고 송신자의 네트워크 주소를 반환합니다.
         *
         * @param data 수신한 데이터를 저장할 버퍼.
         * @param data_length 수신할 데이터의 최대 길이.
         * @param received_length 실제 수신된 바이트 수를 저장할 변수.
         * @param net_address 데이터를 보낸 송신자의 네트워크 주소를 저장할 변수.
         * @return 수신 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool RecvFrom(char* data, int data_length, int& received_length, NetAddress& net_address);

        /**
         * @brief 소켓의 네트워크 주소를 반환합니다.
         *
         * @return 현재 소켓의 네트워크 주소 (IP 및 포트).
         */
        inline NetAddress GetAddress() const { return net_address_; }

    private:
        SOCKET internal_socket_;
        NetAddress net_address_;
    };
}   // namespace Net