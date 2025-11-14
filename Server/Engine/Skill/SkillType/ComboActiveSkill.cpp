#include "pch.h"
#include "ComboActiveSkill.h"

#include <CustomPacket.h>

#include "Skill/SkillType/GeneralAttackSkill.h"
#include "DataManager.h"
#include "NetDef.h"
#include "Map/PlayerCharacter.h"

ComboActiveSkill::ComboActiveSkill(PlayerCharacter* owner, const SkillData* data, int32_t level)
    : ActiveSkill(owner, data, level),
      combo_idx_(0),
      last_input_time_(-1000.f)
{
    for (uint32_t sub_id : data_->combo_skills)
    {
        const SkillData* sub_data = DataManager::Get()->GetSkill(sub_id);
        if (sub_data)
            combo_skills_.push_back(std::make_unique<GeneralAttackSkill>(owner_, sub_data, level_));
    }
}

void ComboActiveSkill::Start()
{
    if (IsCoolDown())
        return;

    is_start_ = true;
    OnStart();
}

void ComboActiveSkill::OnStart()
{
    if (combo_skills_.empty())
        return;

    StartNextCombo();
}

void ComboActiveSkill::StartNextCombo()
{
    float now = Net::GetClientTime();

    if (combo_idx_ >= combo_skills_.size())
    {
        combo_idx_ = 0;
        SetCoolDown(now);
        Stop();
        return;
    }

    if (combo_idx_ != 0)
    {
        auto& prev = combo_skills_[combo_idx_ - 1];
        if (prev->IsCoolDown())
            return;
    }

    auto& combo = combo_skills_[combo_idx_];
    combo->Start();
    combo->Tick(0.f);
    
    last_input_time_ = now;
    combo_idx_++;

    if (combo_idx_ >= combo_skills_.size())
    {
        ResetCombo();
        Stop();
        return;
    }
    else
    {
        ComboSkillStateChangedPacket packet;
        packet.skill_id = GetID();
        packet.combo_idx = combo_idx_;
        packet.cooldown_expired_time = combo->GetCoolDownExpireTime();
        owner_->SendPacket(packet);
    }
}

void ComboActiveSkill::Tick(float delta_time)
{
    if (!IsStart())
        return;

    float now = Net::GetClientTime();

    if (now - last_input_time_ > data_->reset_time)
    {
        ResetCombo();
        Stop();
    }

    for (auto& combo : combo_skills_)
    {
        combo->Tick(delta_time);
    }
}

void ComboActiveSkill::ResetCombo()
{
    float now = Net::GetClientTime();
    combo_idx_ = 0;
    SetCoolDown(now);
    
    ComboSkillStateChangedPacket packet;
    packet.skill_id = GetID();
    packet.combo_idx = combo_idx_;
    packet.cooldown_expired_time = GetCoolDownExpireTime();
    owner_->SendPacket(packet);
}

void ComboActiveSkill::OnStop()
{
}
