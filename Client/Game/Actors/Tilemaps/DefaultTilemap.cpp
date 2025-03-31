#include "pch.h"
#include "DefaultTilemap.h"

#include "Actor/Component/Tilemap/TilemapComponent.h"

DefaultTilemap::DefaultTilemap(const std::wstring& kName) :
    Actor(kName)
{
    tilemap_ = AddComponent<TilemapComponent>(L"Tilemap");
    tilemap_->LoadMap(".\\Content\\Tilemaps\\Default.tmx");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<DefaultTilemap>("DefaultTilemap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
