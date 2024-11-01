#pragma once
#include "Slot.h"
#include "Actor/Actor.h"

class Inventory : public Actor
{
    SHADER_CLASS_HELPER(Inventory)
    GENERATED_BODY(Inventory, Actor);
    
public:
    Inventory(const std::wstring& kName);
    virtual ~Inventory() override = default;

private:
    std::vector<std::unique_ptr<Slot>> slots_;

    int slot_count_;
    int row_count_;
    
};
