#pragma once
#include "Skill/Skill.h"

class PassiveSkill : public Skill
{
public:
    PassiveSkill(PlayerCharacter* owner, const SkillData* data, int32_t level);\
    void Tick(float delta_time) override{}
protected:
    void OnStart() override;
    void OnStop() override;
};