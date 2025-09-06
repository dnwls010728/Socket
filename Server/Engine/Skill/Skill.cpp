#include "pch.h"
#include "Skill.h"

#include "NetDef.h"
#include "Map/PlayerCharacter.h"
#include "Map/MapObject.h"
#include "Map/Map.h"

Skill::Skill(PlayerCharacter* owner, const SkillData* data, bool is_passive, int32_t level)
    : owner_(owner), data_(data), is_passive_(is_passive), last_used_time_(-1000.f), level_(level), is_start_(false)
{
}

uint32_t Skill::GetID() const
{
    return data_ ? data_->id : 0;
}

void Skill::SetCoolDown(float cool_down)
{
    last_used_time_ = cool_down;
}

float Skill::GetCoolDown() const
{
    return static_cast<float>(data_ ? data_->cooldown : 0);
}

bool Skill::IsCoolDown() const
{
    return GetCoolDownLeft() < GetCoolDown();
}

float Skill::GetCoolDownLeft() const
{
    float now = Net::GetClientTime();
    return now - last_used_time_;
}

void Skill::Start()
{
    if (IsCoolDown()) return;

    is_start_ = true;
    float now = Net::GetClientTime();
    SetCoolDown(now);
    OnStart();
}

void Skill::Stop()
{
    is_start_ = false;
    OnStop();
}
