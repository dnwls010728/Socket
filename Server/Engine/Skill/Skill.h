#pragma once
#include <cstdint>
#include "DataManager.h"

class PlayerCharacter;
class MapObject;

class Skill
{
public:
    Skill(PlayerCharacter* owner, const SkillData* data, bool is_passive, int32_t level);
    virtual ~Skill() = default;

    virtual void Start() final;
    virtual void Stop() final;
    inline bool IsStart() const { return is_start_; }
    virtual void Tick(float delta_time) = 0;
    
    void SetOwner(PlayerCharacter* owner) {  owner_ = owner; }
    PlayerCharacter* GetOwner() const { return owner_; }

    uint32_t GetID() const;
    
    inline bool IsPassive() const { return is_passive_; }

    inline void SetCoolDown(float cool_down);
    inline float GetCoolDown() const;
    inline bool IsCoolDown() const;
    inline float GetCoolDownLeft() const;
    
    inline int32_t GetLevel() const { return level_; }
    inline void SetLevel(int32_t level) { level_ = level; }

protected:
    virtual void OnStart() = 0;
    virtual void OnStop() = 0;
    
    PlayerCharacter* owner_;
    
    const SkillData* data_;
    bool is_passive_;
    float last_used_time_;
    int32_t level_;
    bool is_start_;
};
