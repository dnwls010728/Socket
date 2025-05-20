#pragma once
#include <map>
#include <memory>

#include "Item.h"

class Inventory
{
public:
    Inventory();
    ~Inventory() = default;

    void AddSlot(std::unique_ptr<Item> item);

private:
    std::map<uint32_t, std::unique_ptr<Item>> items_;
    
};
