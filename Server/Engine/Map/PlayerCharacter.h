#pragma once
#include <mutex>

#include "MapObject.h"
#include "Utils/TimedBool.h"

class Portal;

namespace Net
{
    struct IPacket;
}

class Inventory;
class Player;

class PlayerCharacter : public MapObject
{
public:
    PlayerCharacter();
    virtual ~PlayerCharacter() override;

    static std::shared_ptr<PlayerCharacter> LoadCharacter(uint32_t character_id, const std::shared_ptr<Player>& player);
    static std::shared_ptr<PlayerCharacter> CreateCharacter(const std::shared_ptr<Player>& player);

    static bool DeleteCharacter(uint32_t character_id);
    
    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    void TakeDamage(int32_t damage_amount);

    bool Disconnect();

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline bool IsInvincible() const { return is_invincible_ == true; }

    inline uint32_t GetAccountID() const { return account_id_; }

    inline void SetName(const std::wstring& name) { name_ = name; }
    inline const std::wstring& GetName() const { return name_; }

    inline void SetBodyColor(const std::wstring& color) { body_color_ = color; }
    inline const std::wstring& GetBodyColor() const { return body_color_; }
    
    inline void SetMapID(int32_t map_id) { map_id_ = map_id; }
    inline int32_t GetMapID() const { return map_id_; }

    inline bool IsMapTransitioning() const { return map_transitioning_.load(); }

    inline void SetPartyID(int32_t party_id) { party_id_ = party_id; }
    inline uint32_t GetPartyID() const { return party_id_; }
    
    inline Inventory* GetInventory() const { return inventory_.get(); }

protected:
    friend class ServerManager;
    friend class Map;
    friend class Player;
    friend class Mob;

    void ChangeMap(Map* to, Portal* to_portal);
    void Respawn();
    void ExitMap();
    void UpdateDatabase();
    void GainExp(int32_t amount);

    virtual void Tick(float delta_time) override;
    
    std::weak_ptr<Player> player_;
    
    uint32_t account_id_;
    uint32_t party_id_;

    std::wstring name_;
    std::wstring body_color_;

    int32_t map_id_;
    int32_t lv_;
    int32_t hp_;
    int32_t max_hp_;

    std::atomic_bool map_transitioning_;
    
    std::atomic_int32_t exp_;
    std::atomic_int32_t color_;

    std::unique_ptr<Inventory> inventory_;
    
    TimedBool is_invincible_;

    std::mutex dropped_item_mutex_;
};
