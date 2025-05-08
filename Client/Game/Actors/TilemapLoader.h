#pragma once
#include "Actor/Actor.h"

class TilemapLoader : public Actor
{
    SHADER_CLASS_HELPER(TilemapLoader)
    GENERATED_BODY(TilemapLoader, Actor)
    
public:
    TilemapLoader(const std::wstring& kName);
    virtual ~TilemapLoader() override = default;

    void SetTilemap(class Tilemap* tilemap);

    int GetType(b2ShapeId shape_id);

protected:
#pragma region 컴포넌트
    std::shared_ptr<class TilemapComponent> tilemap_;
#pragma endregion
    
};
