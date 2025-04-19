#include "Session.h"

#include "../ServerManager.h"

Session::Session(int client_id) :
    client_id_(client_id),
    player_(nullptr)
{
}

void Session::SendPacket(const Net::IPacket& packet) const
{
    if (client_id_ == 0) return;
    ServerManager::Get()->SendPacket(client_id_, packet);
}

void Session::ReceivePacket(Net::IPacket* packet) const
{
    if (player_) player_->ReceivePacket(packet);
}

Player* Session::CreatePlayer(uint32_t account_unique_id)
{
    player_ = std::make_unique<Player>(this, account_unique_id);
    return player_.get();
}

uint32_t Session::GetAccountUniqueID() const
{
    if (player_) return player_->GetAccountUniqueID();
    return 0;
}
