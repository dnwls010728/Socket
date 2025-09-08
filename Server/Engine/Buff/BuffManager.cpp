#include "pch.h"
#include "BuffManager.h"

#include <CustomPacket.h>

#include "Buff.h"
#include "NetDef.h"
#include "Map/PlayerCharacter.h"

BuffManager::BuffManager(PlayerCharacter* owner) :
    owner_(owner),
    active_buffs_(),
    total_stats_()
{
}

bool BuffManager::UseBuff(int32_t buff_id)
{
    const auto& buff = Buff::FindBuff(buff_id);
    if (!buff) return false;

    int32_t hp_delta = 0;
    hp_delta += buff->GetHP();
    hp_delta += owner_->GetMaxHP() * buff->GetHPPercent() / 100;
    if (hp_delta != 0) owner_->ApplyHPDelta(hp_delta);

    float duration = static_cast<float>(buff->GetDuration());
    if (duration <= 0) return false;
    
    float now = Net::GetClientTime();
    
    PlayerBuffPacket packet;
    packet.effect_id = buff->GetBuffID();
    packet.duration = duration;
    packet.server_time = now;
    owner_->SendPacket(packet);

    auto& slot = active_buffs_[buff->GetBuffID()];
    if (!slot.buff)
    {
        slot.buff = buff;
        total_stats_ += buff->GetStatBlock();
    }

    slot.end_time = now + duration;
    return true;
}

bool BuffManager::CheckBuffExpires()
{
    float now = Net::GetClientTime();
    bool is_dirty = false;
    
    auto it = active_buffs_.begin();
    while (it != active_buffs_.end())
    {
        auto& slot = it->second;
        if (slot.end_time > now)
        {
            ++it;
            continue;
        }

        total_stats_ -= slot.buff->GetStatBlock();
        it = active_buffs_.erase(it);
        is_dirty = true;
    }

    return is_dirty;
}
