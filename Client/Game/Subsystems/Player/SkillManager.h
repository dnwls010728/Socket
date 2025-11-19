#pragma once
#include "CommonObject.h"

#include <unordered_map>

class SkillManager
{
public:
    SkillManager() = default;

    void AddOrUpdateSkill(uint32_t skill_id, int32_t level, float cooldown);
    void UseSkill(uint32_t skill_id, float cooldown_expired_time);
    
    void RemoveSkill(uint32_t skill_id);
    void ClearSkills();
    bool HasSkill(uint32_t skill_id) const;
    
    bool CanUseSkill(uint32_t skill_id) const;

    bool IsCoolDown(uint32_t skill_id) const;

    float GetCooldown(uint32_t skill_id) const;
    void SetCooldown(uint32_t skill_id, float cooldown);

    float GetCoolDownLeft(uint32_t skill_id) const;
    void SetCoolDownLeft(uint32_t skill_id, float cool_down_left);

    void SetExpireTime(uint32_t skill_id, float duration);
    float GetExpireTime(uint32_t skill_id) const;

    void SetSkillComboIndex(uint32_t skill_id, int32_t combo_index);
    int32_t GetSkillComboIndex(uint32_t skill_id) const;

    std::vector<SkillInfo> GetSkillList() const;
private:
    struct SkillState
    {
        int32_t level = 1;
        float cooldown = 0.0f;
        float expire_time = 0.0f;
        int32_t combo_index = 0;
    };
    std::unordered_map<uint32_t, SkillState> skills_;
};

