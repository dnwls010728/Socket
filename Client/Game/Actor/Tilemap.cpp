#include "pch.h"
#include "Tilemap.h"
#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "rttr/registration.h"
#include "rttr/detail/registration/registration_impl.h"

Tilemap::Tilemap(const std::wstring& kName) : Actor(kName)
{
    SetLayer(ActorLayer::kGround);
    
    tilemap_component_ = AddComponent<TilemapComponent>(L"Tilemap");
    tilemap_component_->LoadMap(".\\Content\\Tilemaps\\Dummy.tmx");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Tilemap>("Tilemap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
 