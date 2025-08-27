#include "pch.h"
#include "StatEffect.h"

#include <CustomPacket.h>

#include "NetDef.h"
#include "PlayerCharacter.h"

StatEffect::StatEffect(int32_t item_id, const ItemData* item_data) :
    id_(item_id)
{
    const auto& effect = item_data->effect;
    hp_ = effect.hp;
    hp_percent_ = effect.hp_percent;
    atk_ = effect.atk;
    def_ = effect.def;
    dig_ = effect.dig;
    duration_ = effect.duration;
    cooldown_ = effect.cooldown;

    is_skill_= false;

    if (duration_ > 0)
    {
        AddStat(BuffStat::kAtk, atk_);
        AddStat(BuffStat::kDef, def_);
        AddStat(BuffStat::kDig, dig_);
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
        float now = Net::GetClientTime();
        float duration = static_cast<float>(duration_);
        
        PlayerBuffPacket packet;
        packet.effect_id = GetBuffID();
        packet.duration = duration;
        packet.server_time = now;
        packet.stat_changes = stat_changes;
        target->SendPacket(packet);
        
        target->RegisterEffect(
            std::dynamic_pointer_cast<StatEffect>(shared_from_this()),
            now, now + duration
        );
    }
}

int32_t StatEffect::GetBuffID() const
{
    return is_skill_ ? id_ : -id_;
}

void StatEffect::AddStat(BuffStat stat, int32_t value)
{
    if (value == 0) return;
    stat_changes.emplace_back(stat, value);
}
