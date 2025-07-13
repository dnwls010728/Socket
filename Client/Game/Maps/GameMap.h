#pragma once
#include "Level/Level.h"

class GameMap : public Level
{
    SHADER_CLASS_HELPER(GameMap)
    GENERATED_BODY(GameMap, Level)
    
public:
    GameMap(const std::wstring& kName);
    virtual ~GameMap() override = default;

protected:
    virtual void Load() override;
    
};
