#pragma once
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <functional>

#include "Skill.h"
#include "Misc/Function.h"

class Skill;
class PlayerCharacter;
class MapObject;

class SkillManager
{
public:
    SkillManager(PlayerCharacter* owner);

    void AddSkill(uint32_t skill_id, int32_t level);
    bool UseSkill(uint32_t skill_id);
    void Tick(float delta_time);
    
    bool GetSkill(uint32_t skill_id, const std::function<void(Skill* skill)>& callback) const;
    bool HasSkill(uint32_t skill_id) const;
    void RemoveSkill(uint32_t skill_id);
    void ClearSkills();
    void EnumSkills(const Function<void(Skill* skill)>& callback) const;
    // void EnumSkills(const std::function<void(Skill* skill)>& callback) const;

private:
    PlayerCharacter* owner_;
    std::unordered_map<uint32_t, std::unique_ptr<Skill>> skills_;
    mutable std::mutex skills_mutex_;
};

