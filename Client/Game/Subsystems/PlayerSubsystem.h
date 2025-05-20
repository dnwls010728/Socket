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
    std::unique_ptr<Inventory> inventory_;
    
};
