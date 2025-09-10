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
    for (auto& [id, skill] : skills_)
    {
        skill->Tick(delta_time);
    }
}

