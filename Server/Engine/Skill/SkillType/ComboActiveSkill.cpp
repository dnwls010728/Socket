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
      skill_available_time_(-1000.f)
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

    skill_available_time_ = -1000.f;
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
    for (auto& combo : combo_skills_)
    {
        if (combo->IsStart())
            combo->Tick(delta_time);
    }
    
    if (IsStart())
    {
        float now = Net::GetClientTime();
        
        bool prev_ready = true;

        if (combo_idx_ > 0)
        {
            auto& prev = combo_skills_[combo_idx_ - 1];
            prev_ready = !prev->IsCoolDown();
        }
        
        if (skill_available_time_ < 0 && prev_ready)
        {
            skill_available_time_ = now;
            return;
        }
        
        if (skill_available_time_ >= 0)
        {
            if (now - skill_available_time_ > data_->reset_time)
            {
                ResetCombo();
                Stop();
            }
        }
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
