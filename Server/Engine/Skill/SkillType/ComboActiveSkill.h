#pragma once
#include "Skill/SkillType/ActiveSkill.h"
#include <vector>
#include <memory>

class GeneralAttackSkill;
class ComboActiveSkill : public ActiveSkill
{
public:
    ComboActiveSkill(PlayerCharacter* owner, const SkillData* data, int32_t level);

    void Tick(float delta_time) override;
    void ResetCombo();

protected:
    void OnStart() override;
    void OnStop() override;

    void Start() override;

private:
    void StartNextCombo();

    std::vector<std::unique_ptr<GeneralAttackSkill>> combo_skills_;
    int combo_idx_;
    float last_input_time_;
};
