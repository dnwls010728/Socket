#pragma once
#include <string>
#include <winsock2.h>
#include "NetDef.h"

namespace Net::TCP {
    /*
    TCP Server : server.Create
                 server.Bind
                 server.Listen
                 server.Accept
                 client.Recv
                 client.Send
                 client.Close
                 server.Close

    TCP Client : client.Create
                 client.Connect(serverAddr)
                 client.Send
                 client.Recv
                 client.Close
    */

    /**
     * @brief TCP 네트워크 소켓을 관리하는 클래스.
     */
    class NetTCPSocket
    {
    public:
        /**
         * @brief 생성자.
         */
        NetTCPSocket();

        /**
         * @brief 소멸자.
         */
        ~NetTCPSocket();

        /**
         * @brief 소켓을 생성합니다.
         *
         * Accept로 반환된 소켓은 호출하면 안 됩니다.
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
         * @brief 소켓을 IOCP 핸들과 연결합니다.
         *
         * @param iocp_handle IOCP 핸들. NetworkManager::CreateIOCPHandle()로 생성할 수 있습니다.
         * @return 연결 성공 시 true, 실패 시 false를 반환합니다.
         * @warning 한번 연결된 소켓은 다른 IOCP 핸들로 변경할 수 없습니다.
         */
         //bool RegisterIOCPHandle(HANDLE iocp_handle);

         /**
          * @brief 소켓을 IP 주소와 포트에 바인딩합니다.
          *
          * @param net_address 바인딩할 IP 주소와 포트.
          * @param reuse_addr true일 경우 IP 및 포트 재사용을 허용하여 소켓 종료 후 빠르게 재시작할 수 있습니다.
          * @return 바인딩 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
          */
        bool Bind(const Net::NetAddress& net_address, bool reuse_addr);

        /**
         * @brief 연결을 시도하는 소켓을 대기 큐에 추가하기 시작합니다.
         *
         * @param backlog 대기 큐의 최대 길이 (기본값: SOMAXCONN).
         * @return 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Listen(int backlog = SOMAXCONN);

        /**
         * @brief 클라이언트 소켓을 수락하고, 연결된 클라이언트의 소켓을 반환합니다.
         *
         * 클라이언트가 접속할 때까지 대기합니다.
         *
         * @param[out] client_socket 수락된 클라이언트 소켓을 저장할 NetTCPSocket 객체.
         * @return 연결 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Accept(NetTCPSocket& client_socket);

        /**
         * @brief 서버에 연결합니다.
         *
         * @param[in] net_address 연결할 원격 주소 (IP 및 포트).
         * @return 연결 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Connect(const Net::NetAddress& net_address);

        /**
         * @brief 소켓을 통해 데이터를 전송합니다.
         *
         * 기본적으로 논블록 모드로 동작하며, 상대방의 버퍼가 가득 차면 블록될 수 있습니다.
         *
         * @param[in] data 전송할 데이터.
         * @param[in] data_length 전송할 데이터의 길이.
         * @param[out] sent_length 실제 전송된 바이트 수를 저장할 변수.
         * @return 전송 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Send(const char* data, int data_length, int& sent_length);

        /**
         * @brief 소켓을 통해 데이터를 수신합니다.
         *
         * 기본적으로 블록 모드로 동작하며, SetNonBlockingMode(true)를 호출하여 논블록 모드로 전환할 수 있습니다.
         *
         * @param[out] data 수신한 데이터를 저장할 버퍼.
         * @param[in] data_length 수신할 데이터의 길이.
         * @param[out] received_length 실제 수신된 바이트 수를 저장할 변수.
         * @return 수신 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
        bool Recv(char* data, int data_length, int& received_length);

        /**
         * @brief IOCP 방식으로 데이터를 수신합니다.
         *
         * RegisterIOCPHandle()로 IOCP 핸들을 등록한 후, GetTCPIOStatus()로 결과를 확인해야 합니다.
         *
         * @param[in] data_length 수신할 데이터의 길이.
         * @return 수신 성공 시 true, 실패 시 false를 반환합니다. 실패 시 WSAGetLastError()로 오류를 확인할 수 있습니다.
         */
         //bool RecvIOCP(int data_length);

         /**
          * @brief 소켓의 네트워크 주소를 반환합니다.
          *
          * @return 현재 소켓의 네트워크 주소 (IP 및 포트).
          */
        inline Net::NetAddress GetAddress() const { return net_address_; }


        /**
        * @brief 소켓의 내부 핸들을 반환합니다.
        *
        * @return 내부 소켓 핸들.
        */
        SOCKET GetInternalSocket() const { return internal_socket_; }

    private:
        SOCKET internal_socket_; ///< 내부 소켓 핸들.
        Net::NetAddress net_address_; ///< 소켓의 네트워크 주소.
    };

    /**
     * @brief IOCP 컨텍스트 구조체.
     *
     * IOCP 방식의 비동기 작업에서 사용됩니다.
     */
    /*
    struct TCPIOContext
    {
        OVERLAPPED overlapped; ///< IO 완료 통지에 사용되는 OVERLAPPED 구조체.
        NetTCPSocket tcp_socket; ///< 해당 IO 작업과 관련된 TCP 소켓.
        WSABUF buffer;         ///< 데이터를 전송하거나 수신하기 위한 버퍼.
    };
    */

} // namespace Net  