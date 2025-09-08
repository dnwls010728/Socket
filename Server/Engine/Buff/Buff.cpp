#include "pch.h"
#include "Buff.h"

#include "Math/Math.h"

std::unordered_map<int32_t, std::shared_ptr<Buff>> Buff::buffs_ = {};

Buff::Buff(const ItemData* data) :
    is_skill_(false)
{
    id_ = data->id;
    
    auto& effect = data->effect;
    hp_ = effect.hp;
    hp_percent_ = effect.hp_percent;

    stat_block_.max_hp = effect.max_hp;
    stat_block_.atk = effect.atk;
    stat_block_.def = effect.def;
    stat_block_.dig = effect.dig;
    
    duration_ = effect.duration;
    cooldown_ = effect.cooldown;
}

std::shared_ptr<Buff> Buff::GetBuff(int32_t buff_id)
{
    auto it = buffs_.find(buff_id);
    if (it != buffs_.end()) return it->second;

    if (Math::Sign(buff_id) < 0)
    {
        const auto& item_data = DataManager::Get()->GetItem(buff_id);
        if (!item_data) return nullptr;

        auto buff = std::make_shared<Buff>(item_data);
        buffs_.emplace(buff_id, buff);
        return buff;
    }

    const auto& skill_data = DataManager::Get()->GetSkill(buff_id);
    if (!skill_data) return nullptr;

    return nullptr;
}
