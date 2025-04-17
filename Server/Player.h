#pragma once
#include <cstdint>

#include "TCPServerSocket.h"

class Player
{
public:
    Player(Net::TCP::TCPServerSocket* server_socket, uint32_t client_id);
    ~Player() = default;

    void SendPacket(const Net::IPacket& packet);

    inline uint32_t GetClientID() const { return client_id_; }

private:
    Net::TCP::TCPServerSocket* server_socket_;
    
    uint32_t client_id_;
    
};
