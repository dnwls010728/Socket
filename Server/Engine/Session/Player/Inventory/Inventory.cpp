#include "pch.h"
#include "Inventory.h"

#include "Item.h"

Inventory::Inventory(PlayerCharacter* owner) :
    owner_(owner),
    items_(),
    capacity_(0),
    mutex_()
{
}

void Inventory::AddItem(uint32_t slot_id, const std::shared_ptr<Item>& item)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (slot_id > capacity_) return;

    item->SetSlot(slot_id);
    items_[slot_id] = item;
}

bool Inventory::IsFull()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.size() >= capacity_;
}
