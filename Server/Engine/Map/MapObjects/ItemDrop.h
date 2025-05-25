#pragma once
#include "Engine/Map/MapObject.h"

class ItemDrop : public MapObject
{
public:
    ItemDrop();
    virtual ~ItemDrop() override = default;

private:
    uint32_t item_id_;
    uint32_t count_;
    
};
