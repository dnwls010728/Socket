#pragma once
#include "Actor/Actor.h"

class PrototypeTilemap : public Actor
{
    SHADER_CLASS_HELPER(PrototypeTilemap)
    GENERATED_BODY(PrototypeTilemap, Actor)
    
public:
    PrototypeTilemap(const std::wstring& kName);
    virtual ~PrototypeTilemap() override = default;

protected:
#pragma region 컴포넌트
    std::shared_ptr<class TilemapComponent> tilemap_;
#pragma endregion
    
};
