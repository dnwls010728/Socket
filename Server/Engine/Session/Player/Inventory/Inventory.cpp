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

void Inventory::SetItemAt(uint32_t slot_id, const std::shared_ptr<Item>& item)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (slot_id > capacity_) return;

    item->SetSlot(slot_id);
    items_[slot_id] = item;
}

void Inventory::Swap(const std::shared_ptr<Item>& first, const std::shared_ptr<Item>& second)
{
    // std::lock_guard<std::mutex> lock(mutex_);
}

int32_t Inventory::GetCount(uint32_t slot_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = items_.find(slot_id);
    if (it != items_.end())
        return it->second->GetCount();

    return 0;
}

bool Inventory::IsFull()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.size() >= capacity_;
}
