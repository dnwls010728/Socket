#include "pch.h"
#include "MobBase.h"

#include <CommonObject.h>

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "imgui/imgui.h"
#include "Math/Math.h"
#include "Subsystems/NetworkSubsystem.h"

MobBase::MobBase(const std::wstring& name) :
    ServerActor(name),
    is_dead_(false),
    fade_state_(FadeState::kNone),
    fade_timer_(0.f)
{
    SetLayer(ActorLayer::kMob);

    // 임시
    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Mobs\\MushroomSheet.png.animpack");
    if (animation_pack) animator_->SetAnimationPack(animation_pack);
    
    animator_->PlayAnimation(L"Idle");
    
}

void MobBase::OnActivate()
{
    if (HasBegunPlay()) SetActive(true);
}

void MobBase::OnDeactivate()
{
    OnDeath();
}

void MobBase::OnDeath()
{
    animator_->PlayAnimation(L"Die");
    
    fade_state_ = FadeState::kFadeOut;
    fade_timer_ = 0.f;
}

void MobBase::OnEnable()
{
    ServerActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
    
    renderer_->SetFlipX(false);
    animator_->PlayAnimation(L"Idle");

    fade_state_ = FadeState::kFadeIn;
    fade_timer_ = 0.f;
}

void MobBase::OnDisable()
{
    ServerActor::OnDisable();
    NetworkSubsystem::Get()->UnregisterNetworkActor(GetSharedThis());
}

void MobBase::Tick(float delta_time)
{
    ServerActor::Tick(delta_time);

    if (fade_state_ != FadeState::kNone)
    {
        fade_timer_ += delta_time;

        float t = Math::Clamp(fade_timer_ / 1.f, 0.f, 1.f);
        t = (fade_state_ == FadeState::kFadeIn) ? t : (1.f - t);

        Math::Color color = renderer_->GetColor();
        color.a = static_cast<uint8_t>(t * 255.f);
        renderer_->SetColor(color);

        if (t >= 1.f)
        {
            if (fade_state_ == FadeState::kFadeOut)
            {
                is_dead_ = true;
                SetActive(false);
            }
            
            fade_state_ = FadeState::kNone;
            fade_timer_ = 0.f;
        }
    }
}

void MobBase::OnState(uint8_t state, bool is_flipped)
{
    ServerActor::OnState(state, is_flipped);

    MobState mob_state = static_cast<MobState>(state);
    switch (mob_state)
    {
    case MobState::kIdle: animator_->PlayAnimation(L"Idle");
        break;
    case MobState::kWalk: animator_->PlayAnimation(L"Walk");
        break;
    case MobState::kHit: animator_->PlayAnimation(L"Hit");
        break;
    }

    renderer_->SetFlipX(is_flipped);
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
