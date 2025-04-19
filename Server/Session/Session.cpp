#include "Session.h"

#include "Player.h"
#include "../ServerManager.h"

Session::Session(int client_id) :
    client_id_(client_id),
    player_(nullptr)
{
}

void Session::SendPacket(const Net::IPacket& packet)
{
    if (client_id_ == 0) return;
    ServerManager::Get()->SendPacket(client_id_, packet);
}

std::shared_ptr<Player> Session::CreatePlayer(uint32_t account_unique_id)
{
    player_ = std::make_shared<Player>(this, account_unique_id);
    return player_;
}

uint32_t Session::GetAccountUniqueID() const
{
    if (player_) return player_->GetAccountUniqueID();
    return 0;
}
