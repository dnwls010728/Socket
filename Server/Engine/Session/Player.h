#pragma once
#include <CommonObject.h>
#include <cstdint>
#include <memory>

#include "Math/Vector2.h"

class PlayerCharacter;
class Inventory;

namespace Net
{
    struct IPacket;
}

class Session;

class Player : public std::enable_shared_from_this<Player>
{
public:
    Player(Session* session, uint32_t account_id);
    ~Player();

    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    
    void Update();

    void ExitMap();

    bool Disconnect();

    std::vector<std::shared_ptr<PlayerCharacter>> GetCharacters();
    
    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountID() const { return account_id_; }

private:
    Session* session_;

    uint32_t account_id_;

    std::shared_ptr<PlayerCharacter> player_character_;
    
};
