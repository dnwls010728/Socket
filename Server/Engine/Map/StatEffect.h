#pragma once
#include <CommonObject.h>
#include <memory>

#include "DataManager.h"

class PlayerCharacter;

class StatEffect : public std::enable_shared_from_this<StatEffect>
{
public:
    StatEffect(uint32_t item_id, const ItemData* item_data);
    virtual ~StatEffect() = default;

    void Apply(const std::shared_ptr<PlayerCharacter>& target);

    FORCEINLINE uint32_t GetID() const { return id_; }

    FORCEINLINE const std::vector<std::pair<BuffStat, int32_t>>& GetStatChanges() const { return stat_changes; }

private:
    void AddStat(BuffStat stat, int32_t value);
    
    uint32_t id_;
    
    int32_t hp_;
    int32_t hp_percent_;
    int32_t atk_;
    int32_t duration_;
    int32_t cooldown_;

    std::vector<std::pair<BuffStat, int32_t>> stat_changes;
    
};
