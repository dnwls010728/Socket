#pragma once
#include <unordered_map>
#include <memory>
#include <cstdint>
#include "Skill.h"

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

private:
    PlayerCharacter* owner_;
    std::unordered_map<uint32_t, std::unique_ptr<Skill>> skills_;
};

