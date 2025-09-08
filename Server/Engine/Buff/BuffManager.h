#pragma once
#include <memory>
#include <unordered_map>

#include "BuffStatBlock.h"

class Buff;
class PlayerCharacter;

class BuffManager
{
private:
    struct ActiveBuff
    {
        std::shared_ptr<const Buff> buff = nullptr;
        float end_time = 0;
    };
    
public:
    BuffManager(PlayerCharacter* owner);
    ~BuffManager() = default;

    bool UseBuff(int32_t buff_id);
    bool CheckBuffExpires();

    inline const BuffStatBlock& GetTotalStats() const { return total_stats_; }

private:
    PlayerCharacter* owner_;

    std::unordered_map<int32_t, ActiveBuff> active_buffs_;
    BuffStatBlock total_stats_;
    
};
