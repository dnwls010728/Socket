#include "pch.h"
#include "Dummy.h"

#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/Animation.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

Dummy::Dummy(const std::wstring& kName) :
    MobBase(kName)
{
    AssetManager* asset_manager = AssetManager::Get();
    sprite_ = asset_manager->Load<Sprite>(L"Sprites\\Character\\Mob\\Dummy\\DummySheet.png");
    animation_pack_ = asset_manager->Load<AnimationPack>(L"Sprites\\Character\\Mob\\Dummy\\DummySheet.png.animpack");

    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});
    
    if (sprite_) renderer_->SetSprite(sprite_, L"DummySheet_5");

    animator_->SetAnimationPack(animation_pack_);
    animator_->PlayAnimation(L"Idle");

    is_infinite_hp_ = true;
}

void Dummy::Tick(float delta_time)
{
    MobBase::Tick(delta_time);

    if (animator_->GetCurrentAnimation()->GetName() == L"Hit" && !animator_->IsPlaying())
    {
        animator_->PlayAnimation(L"Idle");
    }
}

void Dummy::OnHit()
{
    MobBase::OnHit();

    animator_->PlayAnimation(L"Hit");
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
