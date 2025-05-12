#pragma once
#include <CommonObject.h>
#include <cstdint>

class Map;

namespace Net
{
    struct IPacket;
}

class Session;

class Player
{
    friend class Map;
public:
    Player(Session* session, uint32_t account_unique_id);
    ~Player();

    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    void SetPosition(float x, float y);
    
    inline uint32_t GetCharacterUniqueID() const { return character_unique_id_; }

    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountUniqueID() const { return account_unique_id_; }
    inline Map* GetMap() const { return map_; }

    inline const CharacterInfo& GetCharacterInfo() const { return character_info_; }

    inline float GetPositionX() const { return position_x_; }
    inline float GetPositionY() const { return position_y_; }

private:
    inline void SetMap(Map* kMap) { map_ = kMap; }
    inline void SetCharacterUniqueID(uint32_t character_unique_id) { character_unique_id_ = character_unique_id; }
    
    Session* session_;

    uint32_t account_unique_id_;
    uint32_t character_unique_id_;

    Map* map_;

    CharacterInfo character_info_;

    float position_x_;
    float position_y_;
    
};
