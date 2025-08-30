#pragma once
#include "Level/Level.h"

class TilemapLoader;

class GameMap : public Level
{
    SHADER_CLASS_HELPER(GameMap)
    GENERATED_BODY(GameMap, Level)
    
public:
    GameMap(const std::wstring& kName);
    virtual ~GameMap() override = default;

protected:
    virtual void Load() override;
    virtual void Unload(EndPlayReason type) override;

    virtual void Tick(float deltaTime) override;

private:
    std::shared_ptr<TilemapLoader> tilemap_loader_;
    
};
