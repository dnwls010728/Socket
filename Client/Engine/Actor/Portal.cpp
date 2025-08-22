#include "pch.h"
#include "Portal.h"

#include "Asset/AssetManager.h"
#include "Component/BoxColliderComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/Animator/AnimatorComponent.h"

Portal::Portal(const std::wstring& name) :
    Actor(name),
    id_(-1)
{
    SetLayer(ActorLayer::kPortal);
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    animator_ = AddComponent<AnimatorComponent>(L"Animator");
    
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetOffset({0.f, 1.f});
    collider_->SetSize({2.f, 2.f});
    collider_->SetTrigger(true);
    
    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Portal.png.apack");
    if (animation_pack) animator_->SetAnimationPack(animation_pack);
}

void Portal::BeginPlay()
{
    Actor::BeginPlay();

    animator_->PlayAnimation(L"Idle");
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Portal>("Portal")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
