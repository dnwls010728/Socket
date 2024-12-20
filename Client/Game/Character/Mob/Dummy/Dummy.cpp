#include "pch.h"
#include "Dummy.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

Dummy::Dummy(const std::wstring& kName) :
    MobBase(kName)
{
    AssetManager* asset_manager = AssetManager::Get();
    sprite_ = asset_manager->Load<Sprite>(L"Sprites\\Character\\Mob\\Dummy\\DummySheet.png");

    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});
    
    if (sprite_) renderer_->SetSprite(sprite_, L"DummySheet_5");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Dummy>("Dummy")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
