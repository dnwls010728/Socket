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

    FORCEINLINE const std::wstring& GetName() const { return name_; }

    FORCEINLINE Math::Vector2 GetInitialPosition() const { return { initial_position_x_, initial_position_y_ }; }
    
    FORCEINLINE Inventory* GetInventory() const { return inventory_.get(); }

    static PlayerSubsystem* Get();

private:
    friend class SelectCharacterHandler;
    
    uint32_t account_id_;
    uint32_t character_id_;
    uint32_t lv_;
    uint32_t hp_;
    uint32_t max_hp_;
    uint32_t exp_;

    std::wstring name_;
    
    float initial_position_x_;
    float initial_position_y_;
    
    std::unique_ptr<Inventory> inventory_;
    
};
