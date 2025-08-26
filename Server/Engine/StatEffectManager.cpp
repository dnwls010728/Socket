#include "pch.h"
#include "StatEffectManager.h"

#include "DataManager.h"
#include "Map/StatEffect.h"

StatEffectManager::StatEffectManager() :
    item_effects_()
{
}

std::shared_ptr<StatEffect> StatEffectManager::FindItemEffect(uint32_t item_id)
{
    auto it = item_effects_.find(item_id);
    if (it != item_effects_.end()) return it->second;

    const auto* item_data = DataManager::Get()->GetItem(item_id);
    if (!item_data) return nullptr;

    std::shared_ptr<StatEffect> effect = std::make_shared<StatEffect>(item_id, item_data);
    item_effects_.emplace(item_id, effect);
    return effect;
}
