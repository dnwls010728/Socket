#include "pch.h"
#include "Item.h"

Item::Item(uint32_t id, int32_t count) :
    id_(id),
    count_(count)
{
}

std::shared_ptr<Item> Item::Create(uint32_t id, int32_t count)
{
    std::shared_ptr<Item> item = std::make_shared<Item>(id, count);
    return item;
}
