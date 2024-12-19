#include "pch.h"
#include "Dummy.h"

#include "Logger.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actor/ObjectPool/Bullet.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

Dummy::Dummy(const std::wstring& kName) :
    MobBase(kName)
{
    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Character\\Mob\\Dummy\\DummySheet.png");
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Dummy\\DummySheet.png.animpack");

    animator_ = AddComponent<AnimatorComponent>(L"Animator");
    animator_->SetAnimationPack(animation_pack_);
    animator_->PlayAnimation(L"Idle");
    
    collider_->SetOffset({0.f, 1.f});
}

void Dummy::OnDamaged(float damage)
{
    MobBase::OnDamaged(damage);
    
    rigid_body_->AddForce(Math::Vector2::Right() * 10.f, ForceMode::kImpulse);

    animator_->PlayAnimation(L"Hit");
}

void Dummy::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    if (animator_->GetAnimationName() == L"Hit" && !animator_->IsPlaying())
    {
        animator_->PlayAnimation(L"Idle");
    }
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
