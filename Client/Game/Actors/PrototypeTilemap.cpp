#include "pch.h"
#include "PrototypeTilemap.h"

#include "Actor/Component/Tilemap/TilemapComponent.h"

PrototypeTilemap::PrototypeTilemap(const std::wstring& kName) :
    Actor(kName)
{
    tilemap_ = AddComponent<TilemapComponent>(L"Tilemap");
    tilemap_->LoadMap("Content\\Tilemaps\\PrototypeTilemap.tmx");
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
