#pragma once
#include "Subsystem/GameInstanceSubsystem.h"

#include "Inventory/Inventory.h"

class PlayerSubsystem : public GameInstanceSubsystem
{
    GENERATED_BODY(PlayerSubsystem, GameInstanceSubsystem)
    
public:
    PlayerSubsystem();
    virtual ~PlayerSubsystem() override = default;

    static PlayerSubsystem* Get();

    FORCEINLINE Inventory* GetInventory() const { return inventory_.get(); }

private:
    friend class SelectCharacterHandler;
    
    uint32_t account_id_;
    uint32_t character_id_;
    uint32_t lv_;

    std::wstring name_;
    
    float initial_position_x_;
    float initial_position_y_;
    
    std::unique_ptr<Inventory> inventory_;
    
};
