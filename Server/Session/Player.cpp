#include "Player.h"

#include "Session.h"

Player::Player(Session* session, uint32_t account_unique_id) :
    session_(session),
    account_unique_id_(account_unique_id)
{
}

void Player::SendPacket(const Net::IPacket& packet)
{
    if (!session_) return;
    session_->SendPacket(packet);
}
