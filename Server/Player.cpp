#include "Player.h"

Player::Player(Net::TCP::TCPServerSocket* server_socket, uint32_t client_id) :
    server_socket_(server_socket),
    client_id_(client_id)
{
}

void Player::SendPacket(const Net::IPacket& packet)
{
    if (!server_socket_) return;
    server_socket_->SendPacketToClient(client_id_, packet);
}
