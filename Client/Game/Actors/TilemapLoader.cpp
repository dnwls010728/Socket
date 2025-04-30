#include "pch.h"
#include "TilemapLoader.h"

#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Asset/AssetManager.h"

TilemapLoader::TilemapLoader(const std::wstring& kName) :
    Actor(kName)
{
    tilemap_ = AddComponent<TilemapComponent>(L"Tilemap");

}

void TilemapLoader::SetTilemap(class Tilemap* tilemap)
{
    tilemap_->SetTilemap(tilemap);
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
