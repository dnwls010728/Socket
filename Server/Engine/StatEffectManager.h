#pragma once
#include <unordered_map>

#include "Singleton.h"
#include "Map/StatEffect.h"

class StatEffectManager : public Singleton<StatEffectManager>
{
public:
    StatEffectManager();
    virtual ~StatEffectManager() override = default;

    std::shared_ptr<StatEffect> GetItemEffect(uint32_t item_id);

private:
    std::unordered_map<uint32_t, std::shared_ptr<StatEffect>> item_effects_;
    
};
