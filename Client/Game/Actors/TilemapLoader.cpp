#include "pch.h"
#include "TilemapLoader.h"

#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Asset/AssetManager.h"
#include "Components/GridRendererComponent.h"

TilemapLoader::TilemapLoader(const std::wstring& kName) :
    Actor(kName)
{
    tilemap_ = AddComponent<TilemapComponent>(L"Tilemap");
    grid_renderer_ = AddComponent<GridRendererComponent>(L"GridRenderer");

}

void TilemapLoader::SetTilemap(class Tilemap* tilemap) const
{
    tilemap_->SetTilemap(tilemap);
    
    if (tilemap)
    {
        Math::Vector2 map_size = tilemap->GetMapSize();
        grid_renderer_->SetRows(map_size.y);
        grid_renderer_->SetCols(map_size.x);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<TilemapLoader>("TilemapLoader")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
