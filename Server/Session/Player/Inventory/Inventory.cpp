#include "Inventory.h"

Inventory::Inventory() :
    items_()
{
}

void Inventory::AddSlot(std::unique_ptr<Item> item)
{
    if (!item || item->GetSlotIndex() < 1) return;
    items_[item->GetSlotIndex()] = std::move(item);
}
