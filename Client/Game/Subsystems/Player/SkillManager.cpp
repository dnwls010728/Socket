#include "pch.h"
#include "SkillManager.h"

#include "Subsystems/SessionSubsystem.h"

void SkillManager::AddOrUpdateSkill(uint32_t skill_id, int32_t level, float cooldown)
{
    auto& skill = skills_[skill_id];
    skill.level = level;
    skill.cooldown = cooldown;
    skill.expire_time = 0.0;
}

void SkillManager::UseSkill(uint32_t skill_id, float cooldown_expired_time)
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return;

    it->second.expire_time = cooldown_expired_time;
}

void SkillManager::RemoveSkill(uint32_t skill_id)
{
    skills_.erase(skill_id);
}

void SkillManager::ClearSkills()
{
    skills_.clear();
}

bool SkillManager::HasSkill(uint32_t skill_id) const
{
    return skills_.find(skill_id) != skills_.end();
}

bool SkillManager::CanUseSkill(uint32_t skill_id) const
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return false;

    double now = SessionSubsystem::Get()->GetServerTime();
    return now >= it->second.expire_time;
}

bool SkillManager::IsCoolDown(uint32_t skill_id) const
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return false;

    double now = SessionSubsystem::Get()->GetServerTime();
    return now < it->second.expire_time;
}

float SkillManager::GetCooldown(uint32_t skill_id) const
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return 0.0f;

    return it->second.cooldown;
}

void SkillManager::SetCooldown(uint32_t skill_id, float cooldown)
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return;

    it->second.cooldown = cooldown;
}

float SkillManager::GetCoolDownLeft(uint32_t skill_id) const
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return 0.0f;

    double now = SessionSubsystem::Get()->GetServerTime();
    double left = it->second.expire_time - now;
    return static_cast<float>(std::max(0.0, left));
}

void SkillManager::SetCoolDownLeft(uint32_t skill_id, float cool_down_left)
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return;

    double now = SessionSubsystem::Get()->GetServerTime();
    it->second.expire_time = now + cool_down_left;
}

void SkillManager::SetExpireTime(uint32_t skill_id, float duration)
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return;

    double now = SessionSubsystem::Get()->GetServerTime()   ;
    it->second.expire_time = now + duration;
}

float SkillManager::GetExpireTime(uint32_t skill_id) const
{
    auto it = skills_.find(skill_id);
    if (it == skills_.end())
        return 0.0f;

    return it->second.expire_time;
}

std::vector<SkillInfo> SkillManager::GetSkillList() const
{
    std::vector<SkillInfo> skills;
    for (const auto& [skill_id, skill_state] : skills_)
    {
        skills.push_back({ skill_id, skill_state.level, skill_state.cooldown });
    }
    return skills;
}
