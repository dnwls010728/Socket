#pragma once
#include "Skill/Skill.h"

class ActiveSkill : public Skill
{
public:
    ActiveSkill(PlayerCharacter* owner, const SkillData* data, int32_t level) : Skill(owner, data, false, level) {}
};