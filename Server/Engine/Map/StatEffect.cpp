#include "pch.h"
#include "StatEffect.h"

#include "PlayerCharacter.h"

StatEffect::StatEffect(uint32_t item_id, const ItemData* item_data) :
    id_(item_id)
{
    uint32_t type = item_id / 1000000;
    
    const auto& effect = item_data->effect;
    hp_ = effect.hp;
    hp_percent_ = effect.hp_percent;
}

void StatEffect::Apply(const std::shared_ptr<PlayerCharacter>& target)
{
    int32_t hp_delta = 0;
    if (hp_ != 0) hp_delta += hp_;
    if (hp_percent_ != 0) hp_delta += target->GetMaxHP() * hp_percent_ / 100;

    target->ApplyHPDelta(hp_delta);
}
