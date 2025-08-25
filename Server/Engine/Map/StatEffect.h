#pragma once
#include <memory>

#include "DataManager.h"

class PlayerCharacter;

class StatEffect : public std::enable_shared_from_this<StatEffect>
{
public:
    StatEffect(uint32_t item_id, const ItemData* item_data);
    virtual ~StatEffect() = default;

    void Apply(const std::shared_ptr<PlayerCharacter>& target);

private:
    uint32_t id_;
    
    int32_t hp_;
    int32_t hp_percent_;
    
};
