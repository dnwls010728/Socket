#pragma once
#include "Subsystem/GameInstanceSubsystem.h"

#include "Inventory/Inventory.h"
#include "PacketHandlers/PlayerStatsUpdateHandler.h"

class PlayerSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(PlayerSubsystem, GameInstanceSubsystem)
    
public:
    PlayerSubsystem();
    virtual ~PlayerSubsystem() override = default;

    void UpdateStat(PlayerStat stat, uint32_t value);

    FORCEINLINE uint32_t GetAccountID() const { return account_id_; }
    FORCEINLINE uint32_t GetCharacterID() const { return character_id_; }
    FORCEINLINE uint32_t GetLv() const { return lv_; }
    FORCEINLINE uint32_t GetHP() const { return hp_; }
    FORCEINLINE uint32_t GetMaxHP() const { return max_hp_; }
    FORCEINLINE uint32_t GetExp() const { return exp_; }

    FORCEINLINE const std::wstring& GetName() const { return name_; }
    
    FORCEINLINE Inventory* GetInventory() const { return inventory_.get(); }

    static PlayerSubsystem* Get();

private:
    friend class SelectCharacterHandler;
    friend class MapLoadHandler;
    friend class GameMap;
    
    uint32_t account_id_;
    uint32_t character_id_;
    uint32_t lv_;
    uint32_t hp_;
    uint32_t max_hp_;
    uint32_t exp_;
    uint32_t map_id_;

    Math::Vector2 spawn_position;

    std::wstring name_;
    
    std::unique_ptr<Inventory> inventory_;
    
};
