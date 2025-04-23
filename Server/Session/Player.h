#pragma once
#include <CommonObject.h>
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
    void SetPosition(float x, float y);

    inline void SetCharacterUniqueID(uint32_t character_unique_id) { character_unique_id_ = character_unique_id; }
    inline uint32_t GetCharacterUniqueID() const { return character_unique_id_; }

    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountUniqueID() const { return account_unique_id_; }
    inline GameMap* GetMap() const { return map_; }

    inline const CharacterInfo& GetCharacterInfo() const { return character_info_; }

    inline float GetPositionX() const { return position_x_; }
    inline float GetPositionY() const { return position_y_; }

private:
    Session* session_;

    uint32_t account_unique_id_;
    uint32_t character_unique_id_;

    GameMap* map_;

    CharacterInfo character_info_;

    float position_x_;
    float position_y_;
    
};
