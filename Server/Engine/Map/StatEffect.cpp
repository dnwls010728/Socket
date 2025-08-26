#include "pch.h"
#include "StatEffect.h"

#include "NetDef.h"
#include "PlayerCharacter.h"

StatEffect::StatEffect(uint32_t item_id, const ItemData* item_data) :
    id_(item_id)
{
    const auto& effect = item_data->effect;
    hp_ = effect.hp;
    hp_percent_ = effect.hp_percent;
    atk_ = effect.atk;
    duration_ = effect.duration;
    cooldown_ = effect.cooldown;

    if (duration_ > 0)
    {
        AddStat(BuffStat::kAtk, atk_);
    }
}

void StatEffect::Apply(const std::shared_ptr<PlayerCharacter>& target)
{
#pragma region HP 회복
    int32_t hp_delta = 0;
    if (hp_ != 0) hp_delta += hp_;
    if (hp_percent_ != 0) hp_delta += target->GetMaxHP() * hp_percent_ / 100;

    target->ApplyHPDelta(hp_delta);
#pragma endregion

    if (!stat_changes.empty())
    {
        float start_time = Net::GetClientTime();
        target->RegisterEffect(
            std::dynamic_pointer_cast<StatEffect>(shared_from_this()),
            start_time, start_time + duration_
        );
    }
}

void StatEffect::AddStat(BuffStat stat, int32_t value)
{
    if (value == 0) return;
    stat_changes.emplace_back(stat, value);
}
