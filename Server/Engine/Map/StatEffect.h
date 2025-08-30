#pragma once
#include <CommonObject.h>
#include <memory>

#include "DataManager.h"

class PlayerCharacter;

class StatEffect : public std::enable_shared_from_this<StatEffect>
{
public:
    StatEffect(int32_t item_id, const ItemData* item_data);
    virtual ~StatEffect() = default;

    void Apply(const std::shared_ptr<PlayerCharacter>& target);

    int32_t GetBuffID() const;

    FORCEINLINE int32_t GetID() const { return id_; }

    FORCEINLINE const std::vector<std::pair<BuffStat, int32_t>>& GetStatChanges() const { return stat_changes; }

private:
    void AddStat(BuffStat stat, int32_t value);
    
    int32_t id_;
    
    int32_t hp_;
    int32_t hp_percent_;
    int32_t atk_;
    int32_t def_;
    int32_t dig_;
    int32_t duration_;
    int32_t cooldown_;

    std::wstring block_color_;

    bool is_skill_;

    std::vector<std::pair<BuffStat, int32_t>> stat_changes;
    
};
