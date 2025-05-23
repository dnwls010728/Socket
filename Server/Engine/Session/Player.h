#pragma once
#include <CommonObject.h>
#include <cstdint>
#include <memory>

#include "Math/Vector2.h"

class Inventory;
class Map;

namespace Net
{
    struct IPacket;
}

class Session;

class Player : public std::enable_shared_from_this<Player>
{
    friend class Map;
public:
    Player(Session* session, uint32_t account_id);
    ~Player();

    void LoadCharacter(uint32_t unique_id);
    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    
    inline uint32_t GetCharacterUniqueID() const { return character_unique_id_; }
    void Update();

    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountID() const { return account_id_; }
    inline uint32_t GetCharacterID() const { return character_id_; }
    inline Map* GetMap() const { return map_; }

    inline const CharacterInfo& GetCharacterInfo() const { return character_info_; }

    inline void SetPosition(const Math::Vector2 &position) {position_ = position;}
    inline Math::Vector2 GetPosition() const { return position_; }
    
    inline Inventory* GetInventory() const { return inventory_.get(); }

private:
    inline void SetMap(Map* kMap) { map_ = kMap; }
    inline void SetCharacterUniqueID(uint32_t character_unique_id) { character_unique_id_ = character_unique_id; }
    
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

    std::unique_ptr<Inventory> inventory_;
    Math::Vector2 position_;
    
};
