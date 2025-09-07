#pragma once
#include <mutex>
#include <unordered_map>
#include <memory>

#include "MapObject.h"
#include "../../../Client/Engine/Misc/EnumClassFlags.h"
#include "Session/Player.h"
#include "Session/Player/Inventory/Inventory.h"
#include "Utils/TimedBool.h"
#include "Skill/SkillManager.h"

class EquipItem;
class StatEffect;
class Portal;

namespace Net
{
    struct IPacket;
}

class OLD_Inventory;
class Player;
enum class PartyStatType : uint8_t;
class SkillManager;

ENUM_CLASS_FLAGS(PlayerStat)

class PlayerCharacter : public MapObject
{
public:
    struct BuffStatValue
    {
        std::shared_ptr<StatEffect> effect;
        float start_time;
        int32_t value;
    };

    struct EquipStat
    {
        int32_t max_hp;
        int32_t atk;
        int32_t def;
        int32_t dig;
    };
    
    PlayerCharacter();
    virtual ~PlayerCharacter() override;

    static std::shared_ptr<PlayerCharacter> LoadCharacter(uint32_t character_id, const std::shared_ptr<Player>& player);
    static std::shared_ptr<PlayerCharacter> CreateCharacter(const std::shared_ptr<Player>& player);

    static bool DeleteCharacter(uint32_t character_id);
    
    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet);
    void TakeDamage(int32_t damage_amount);
    void ApplyHPDelta(int32_t hp_delta);
    void RegisterEffect(const std::shared_ptr<StatEffect>& effect, float start_time, float expire_time);

    bool Disconnect();

    int32_t GetBuffedValue(BuffStat stat) const;

    virtual void SendSpawn(const std::shared_ptr<PlayerCharacter>& player) override;

    inline bool IsInvincible() const { return is_invincible_ == true; }

    inline uint32_t GetAccountID() const { return account_id_; }

    inline void SetName(const std::wstring& name) { name_ = name; }
    inline const std::wstring& GetName() const { return name_; }

    inline void SetBodyColor(const std::wstring& color) { body_color_ = color; }
    inline const std::wstring& GetBodyColor() const { return body_color_; }
    
    inline void SetMapID(uint32_t map_id) { map_id_ = map_id; }
    inline uint32_t GetMapID() const { return map_id_; }

    inline bool IsMapTransitioning() const { return map_transitioning_.load(); }
    
    inline Inventory* GetInventory(InventoryType type) const { return inventories_[static_cast<uint8_t>(type)].get(); }

    inline const std::array<std::unique_ptr<Inventory>, static_cast<uint8_t>(InventoryType::kCount)>& GetInventories() const { return inventories_; }

    void SetPartyID(int32_t party_id);
    inline uint32_t GetPartyID() const { return party_id_; }

    inline int32_t GetLv() const { return lv_; }
    inline int32_t GetHP() const { return hp_; }
    inline int32_t GetMaxHP() const { return effective_max_hp_; }
    inline bool IsFlipped() const { return is_flipped_; }

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
    void NotifyPartyStatChange(PartyStatType stat, int32_t value, bool exclude_self = false);
    void CheckBuffExpire();
    void RecalcEffectiveStats();
    void AddEquipStats(uint32_t slot, const std::shared_ptr<EquipItem>& item);
    void RemoveEquipStats(uint32_t slot);
    
    bool IsBuffStronger(const BuffStatValue& new_effect, const BuffStatValue& existing_effect) const;

    virtual void Tick(float delta_time) override;
    
    std::weak_ptr<Player> player_;
    
    uint32_t account_id_;
    uint32_t party_id_;

    std::wstring name_;
    std::wstring body_color_;
    std::wstring current_animation_;

    uint32_t map_id_;
    
    int32_t lv_;
    
    int32_t hp_;
    int32_t base_max_hp_;
    
    int32_t equip_max_hp_;
    int32_t equip_atk_;
    int32_t equip_def_;
    int32_t equip_dig_;
    
    int32_t effective_max_hp_;
    int32_t effective_atk_;
    int32_t effective_def_;
    int32_t effective_dig_;

    bool is_dead_;
    bool is_flipped_;
    bool is_placing_;
    
    std::atomic_bool map_transitioning_;
    
    std::atomic_int32_t exp_;
    std::atomic_int32_t color_;

    std::array<std::unique_ptr<Inventory>, static_cast<uint8_t>(InventoryType::kCount)> inventories_;
    SkillManager skill_manager_;
    
    TimedBool is_invincible_;

    std::mutex dropped_item_mutex_;
    std::mutex effect_mutex_;

    std::unordered_map<uint32_t, EquipStat> equip_stats_;
    std::unordered_map<int32_t, std::unordered_map<BuffStat, BuffStatValue>> buff_effects_;
    std::unordered_map<int32_t, float> buff_expires_;
    std::unordered_map<BuffStat, BuffStatValue> effects_;

    float buff_timer_;
};
