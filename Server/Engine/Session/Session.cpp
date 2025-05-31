#include "pch.h"
#include "Session.h"
#include "ServerManager.h"

Session::Session(int client_id) :
    client_id_(client_id),
    state_(State::kNone),
    player_(nullptr)
{
}

Session::~Session()
{
    if (player_)
    {
        player_->ExitMap();
        player_ = nullptr;
    }
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

void Session::Update()
{
    if (player_) player_->Update();
}

std::shared_ptr<Player> Session::CreatePlayer(uint32_t account_id)
{
    if (player_) player_->ExitMap();
    
    player_ = std::make_shared<Player>(this, account_id);
    return player_;
}

uint32_t Session::GetAccountUniqueID() const
{
    if (player_) return player_->GetAccountID();
    return 0;
}
