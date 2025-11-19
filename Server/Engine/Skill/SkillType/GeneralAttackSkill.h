#pragma once
#include "Math/Vector2.h"
#include "Skill/SkillType/ActiveSkill.h"

class AttackChain;
class GeneralAttackSkill : public ActiveSkill
{
public:
    GeneralAttackSkill(PlayerCharacter* owner, const SkillData* data, int32_t level);
    void Tick(float delta_time) override;

protected:
    void OnStart() override;
    void OnStop() override{}
    
private:
    void ApplyHitFrame(const HitFrame& frame);
    
    size_t next_frame_;
    float elapsed_;

    bool is_flipped_;
    Math::Vector2 owner_position_;
};