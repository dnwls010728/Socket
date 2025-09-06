#include "pch.h"
#include "EquipItem.h"

#include "DataManager.h"

EquipItem::EquipItem(uint32_t id) :
    Item(id, 1),
    req_lv_(1),
    max_hp_(0),
    atk_(0),
    def_(0),
    dig_(0)
{
}

std::shared_ptr<Item> EquipItem::Clone() const
{
    return std::make_shared<EquipItem>(*this);
}

std::shared_ptr<EquipItem> EquipItem::Create(uint32_t id)
{
    auto* item_data = DataManager::Get()->GetItem(id);
    if (!item_data) return nullptr;
    
    auto equip = std::make_shared<EquipItem>(id);
    equip->req_lv_ = item_data->stat.req_lv;
    equip->max_hp_ = item_data->stat.max_hp;
    equip->atk_ = item_data->stat.atk;
    equip->def_ = item_data->stat.def;
    equip->dig_ = item_data->stat.dig;
    
    return equip;
}
