#include "pch.h"
#include "Buff.h"

#include "Math/Math.h"

std::unordered_map<int32_t, std::shared_ptr<const Buff>> Buff::cache_ = {};
std::shared_mutex Buff::cache_mutex_ = {};

Buff::Buff(const SkillData* data) :
    is_skill_(true),
    hp_(0),
    hp_percent_(0)
{
    id_ = data->id;

    stat_block_.max_hp = data->max_hp;
    stat_block_.atk = data->atk;
    stat_block_.def = data->def;
    stat_block_.dig = data->dig;

    duration_ = data->duration;
    cooldown_ = data->cooldown;
}

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

std::shared_ptr<const Buff> Buff::FindBuff(int32_t buff_id)
{
    {
        std::shared_lock<std::shared_mutex> lock(cache_mutex_);
        auto it = cache_.find(buff_id);
        if (it != cache_.end()) return it->second;
    }

    std::shared_ptr<const Buff> buff = nullptr;

    if (Math::Sign(buff_id) < 0)
    {
        const auto& item_data = DataManager::Get()->GetItem(Math::Abs(buff_id));
        if (!item_data) return nullptr;
        
        buff = std::make_shared<Buff>(item_data);
    }
    else
    {
        // 추가 예정
        const auto& skill_data = DataManager::Get()->GetSkill(buff_id);
        if (!skill_data) return nullptr;
        
        return nullptr;
    }

    std::unique_lock<std::shared_mutex> lock(cache_mutex_);
    auto [it, _] = cache_.try_emplace(buff_id, std::move(buff));
    return it->second;
}
