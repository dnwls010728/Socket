#include "pch.h"
#include "SkillFactory.h"
#include "Skill.h"
#include "SkillType/GeneralAttackSkill.h"
#include "SkillType/ComboActiveSkill.h"

std::unique_ptr<Skill> SkillFactory::Create(
    PlayerCharacter* owner,
    const SkillData* data,
    int32_t level)
{
    switch (data->type)
    {
    case SkillType::kProjectile:
        return std::make_unique<GeneralAttackSkill>(owner, data, level); // ProjectileSkill 있으면 수정

    case SkillType::kComboAttack:
        return std::make_unique<ComboActiveSkill>(owner, data, level);

    default:
        return std::make_unique<GeneralAttackSkill>(owner, data, level);
    }
}
