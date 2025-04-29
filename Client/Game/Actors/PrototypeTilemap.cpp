#include "pch.h"
#include "PrototypeTilemap.h"

#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Asset/AssetManager.h"

PrototypeTilemap::PrototypeTilemap(const std::wstring& kName) :
    Actor(kName)
{
    tilemap_ = AddComponent<TilemapComponent>(L"Tilemap");

    Tilemap* tilemap_data = AssetManager::Get()->Load<Tilemap>(L"Tilemaps\\PrototypeTilemap.tmx");
    if (tilemap_data) tilemap_->SetTilemap(tilemap_data);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PrototypeTilemap>("PrototypeTilemap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
