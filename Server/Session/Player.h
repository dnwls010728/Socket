#pragma once
#include <cstdint>

class GameMap;

namespace Net
{
    struct IPacket;
}

class Session;

class Player
{
public:
    Player(Session* session, uint32_t account_unique_id);
    ~Player();

    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);

    inline void SetCharacterUniqueID(uint32_t character_unique_id) { character_unique_id_ = character_unique_id; }
    inline uint32_t GetCharacterUniqueID() const { return character_unique_id_; }

    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountUniqueID() const { return account_unique_id_; }
    inline GameMap* GetMap() const { return map_; }

private:
    Session* session_;

    uint32_t account_unique_id_;
    uint32_t character_unique_id_;

    GameMap* map_;
    
};
