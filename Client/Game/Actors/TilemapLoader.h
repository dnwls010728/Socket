#pragma once
#include "Actor/Actor.h"

class GridRendererComponent;
class TilemapComponent;

class TilemapLoader : public Actor
{
    SHADER_CLASS_HELPER(TilemapLoader)
    GENERATED_BODY(TilemapLoader, Actor)
    
public:
    TilemapLoader(const std::wstring& kName);
    virtual ~TilemapLoader() override = default;

    void SetTilemap(class Tilemap* tilemap) const;

    FORCEINLINE const std::shared_ptr<TilemapComponent>& GetTilemapComponent() const { return tilemap_; }

protected:
#pragma region 컴포넌트
    std::shared_ptr<TilemapComponent> tilemap_;
    std::shared_ptr<GridRendererComponent> grid_renderer_;
#pragma endregion
    
};
