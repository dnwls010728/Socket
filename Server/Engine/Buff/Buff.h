#pragma once
#include <cstdint>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "BuffStatBlock.h"
#include "DataManager.h"

class PlayerCharacter;

class Buff
{
public:
    Buff(const SkillData* data);
    Buff(const ItemData* data);
    ~Buff() = default;

    inline int32_t GetID() const { return id_; }
    inline int32_t GetBuffID() const { return is_skill_ ? id_ : -id_; }
    inline int32_t GetHP() const { return hp_; }
    inline int32_t GetHPPercent() const { return hp_percent_; }
    inline int32_t GetDuration() const { return duration_; }
    inline int32_t GetCooldown() const { return cooldown_; }
    
    inline const BuffStatBlock& GetStatBlock() const { return stat_block_; }

    inline bool IsSkill() const { return is_skill_; }

    static std::shared_ptr<const Buff> FindBuff(int32_t buff_id);

private:
    int32_t id_;

    // 즉시 적용
    int32_t hp_;
    int32_t hp_percent_;
    
    int32_t duration_;
    int32_t cooldown_;

    BuffStatBlock stat_block_;

    bool is_skill_;

    static std::unordered_map<int32_t, std::shared_ptr<const Buff>> cache_;
    static std::shared_mutex cache_mutex_;
    
};
