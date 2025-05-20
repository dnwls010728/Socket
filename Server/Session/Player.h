#pragma once
#include <CommonObject.h>
#include <cstdint>
#include <memory>

class Inventory;
class Map;

namespace Net
{
    struct IPacket;
}

class Session;

class Player
{
public:
    Player(Session* session, uint32_t account_id);
    ~Player();

    void LoadCharacter(uint32_t unique_id);
    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    void SetPosition(float x, float y);

    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountUniqueID() const { return account_id_; }
    inline Map* GetMap() const { return map_; }

    inline const CharacterInfo& GetCharacterInfo() const { return character_info_; }

    inline float GetPositionX() const { return position_x_; }
    inline float GetPositionY() const { return position_y_; }

private:
    Session* session_;

    uint32_t account_id_;
    uint32_t character_id_;

    Map* map_;

    CharacterInfo character_info_;

    // 캐릭터 정보
    std::wstring name_;

    int32_t lv_;
    int32_t map_id_;
    int32_t color_;

    float position_x_;
    float position_y_;

    std::unique_ptr<Inventory> inventory_;
    
};
