#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class Image;

struct Slot
{
    Image* slot;
    Image* item;
};

class Inventory : public Actor
{
    SHADER_CLASS_HELPER(Inventory)
    GENERATED_BODY(Inventory, Actor);
    
public:
    Inventory(const std::wstring& kName);
    virtual ~Inventory() override = default;

private:
    void OnItemDrop(const Math::Vector2& kPosition);

    int slot_count_;
    int slot_row_count_;
    
    Image* window_caption_;

    std::vector<Slot> slots_;
    
};
