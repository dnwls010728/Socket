#include "pch.h"
#include "PasiveSkill.h"
#include "Map/PlayerCharacter.h"

PassiveSkill::PassiveSkill(PlayerCharacter* owner, const SkillData* data, int32_t level)
    : Skill(owner, data, true, level)
{
}

void PassiveSkill::OnStart()
{
}

void PassiveSkill::OnStop()
{
}
