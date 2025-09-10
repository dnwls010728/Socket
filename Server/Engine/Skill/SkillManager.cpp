#include "pch.h"
#include "SkillManager.h"

#include "Skill.h"
#include "DataManager.h"
#include "Map/PlayerCharacter.h"
#include "Skill/SkillType/ActiveSkill.h"
#include "Skill/SkillType/PasiveSkill.h"

SkillManager::SkillManager(PlayerCharacter* owner)
    : owner_(owner)
{
}

void SkillManager::AddSkill(uint32_t skill_id, int32_t level)
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    
    const SkillData* data = DataManager::Get()->GetSkill(skill_id);
    if (!data) return;
    
    std::unique_ptr<Skill> skill;
    if (false)
        skill = std::make_unique<PassiveSkill>(owner_, data, level);
    else
        skill = std::make_unique<ActiveSkill>(owner_, data, level);

    skills_.emplace(skill_id, std::move(skill));
}

bool SkillManager::UseSkill(uint32_t skill_id)
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    
    auto it = skills_.find(skill_id);
    if (it == skills_.end()) return false;

    Skill* skill = it->second.get();
    if (skill->IsPassive())
    {
        skill->Start();
        return true;
    }

    if (!skill->IsCoolDown())
    {
        skill->Start();
        skill->Tick(0.f);
        return true;
    }
    return false;
}

void SkillManager::Tick(float delta_time)
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    
    for (auto& [id, skill] : skills_)
    {
        skill->Tick(delta_time);
    }
}

bool SkillManager::GetSkill(uint32_t skill_id, const std::function<void(Skill* skill)>& callback) const
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    
    auto it = skills_.find(skill_id);
    if (it == skills_.end()) return false;

    callback(it->second.get());
    return true;
}

bool SkillManager::HasSkill(uint32_t skill_id) const
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    return skills_.find(skill_id) != skills_.end();
}

void SkillManager::RemoveSkill(uint32_t skill_id)
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    skills_.erase(skill_id);
}

void SkillManager::ClearSkills()
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    skills_.clear();
}

void SkillManager::EnumSkills(const std::function<void(Skill* skill)>& callback) const
{
    std::lock_guard<std::mutex> lock(skills_mutex_);
    for (auto& [id, skill] : skills_)
    {
        callback(skill.get());
    }
}
