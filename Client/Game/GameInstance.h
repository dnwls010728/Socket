#pragma once
#include "Data/PropData.h"

class GameInstance : public Singleton<GameInstance>
{
public:
    GameInstance();
    virtual ~GameInstance() override = default;

    void Init();
    void Shutdown();

    const PropData* GetPropData(int id);

private:
    std::vector<PropData> prop_data_;
    
};
