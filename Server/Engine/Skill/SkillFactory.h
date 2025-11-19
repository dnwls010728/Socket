#pragma once
#include <memory>

struct SkillData;
class PlayerCharacter;
class Skill;

class SkillFactory
{
public:
    static std::unique_ptr<Skill> Create( PlayerCharacter* owner, const SkillData* data, int32_t level);
};
