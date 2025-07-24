#pragma once
#include "MapObject.h"
#include "Utils/TimedBool.h"

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
    
    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    void TakeDamage(int32_t damage_amount);

    bool Disconnect();

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline bool IsInvincible() const { return is_invincible_ == true; }

    inline uint32_t GetAccountID() const { return account_id_; }

    inline const std::wstring& GetName() const { return name_; }
    
    inline Inventory* GetInventory() const { return inventory_.get(); }

protected:
    friend class Map;
    friend class Player;
    friend class Mob;

    void ExitMap();
    void UpdateCharacter();
    void GainExp(int32_t amount);

    virtual void Tick(float delta_time) override;
    
    std::weak_ptr<Player> player_;
    
    uint32_t account_id_;

    std::wstring name_;
    std::wstring character_color_;

    int32_t lv_;
    int32_t hp_;
    int32_t max_hp_;
    
    std::atomic_int32_t exp_;
    std::atomic_int32_t color_;

    std::unique_ptr<Inventory> inventory_;
    
    TimedBool is_invincible_;
};
