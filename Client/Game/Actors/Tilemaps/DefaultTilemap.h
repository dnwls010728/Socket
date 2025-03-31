#pragma once
#include "Actor/Actor.h"

class DefaultTilemap : public Actor
{
    SHADER_CLASS_HELPER(DefaultTilemap)
    GENERATED_BODY(DefaultTilemap, Actor)
    
public:
    DefaultTilemap(const std::wstring& kName);
    virtual ~DefaultTilemap() override = default;

protected:
#pragma region COMPONENTS
    class TilemapComponent* tilemap_;
#pragma endregion
    
};
