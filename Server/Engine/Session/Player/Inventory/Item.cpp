#include "pch.h"
#include "Item.h"

#include "EquipItem.h"

Item::Item(uint32_t id, int32_t count) :
    id_(id),
    slot_(0),
    count_(count)
{
}

std::shared_ptr<Item> Item::Clone() const
{
    return std::make_shared<Item>(*this);
}

std::shared_ptr<Item> Item::Create(uint32_t id, int32_t count)
{
    return std::make_shared<Item>(id, count);
}
