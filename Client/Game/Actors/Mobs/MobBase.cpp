#include "pch.h"
#include "MobBase.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

MobBase::MobBase(const std::wstring& name) :
    ServerActor(name)
{
    SetLayer(ActorLayer::kCharacter);
    
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(10000);
    
    animator_ = AddComponent<AnimatorComponent>(L"Animator");

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Box.png");
    if (sprite) renderer_->SetSprite(sprite, L"Box_0");
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MobBase>("MobBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
