#include "pch.h"
#include "MobBase.h"

#include <CommonObject.h>

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "imgui/imgui.h"
#include "Math/Math.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/NetworkSubsystem.h"

MobBase::MobBase(const std::wstring& name) :
    ServerActor(name),
    is_dead_(false),
    fade_state_(FadeState::kNone),
    fade_timer_(0.f)
{
    SetLayer(ActorLayer::kMob);

    hit_sound_ = AssetManager::Get()->Load<Audio>(L"Audio\\SFX\\damage5.mp3");
    
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
    is_dead_ = true;
    animator_->PlayAnimation(L"Die");
    
    fade_state_ = FadeState::kFadeOut;
    fade_timer_ = 0.f;
}

bool MobBase::Init(uint32_t mob_id) const
{
    const MobData* mob_data = DataSubsystem::Get()->GetMobData(mob_id);
    if (!mob_data) return false;
    
    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(mob_data->animation_pack);
    if (animation_pack) animator_->SetAnimationPack(animation_pack);

    animator_->PlayAnimation(L"Idle");

    return true;
}

void MobBase::OnEnable()
{
    ServerActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
    
    renderer_->SetFlipX(false);
    animator_->PlayAnimation(L"Idle");

    fade_state_ = FadeState::kFadeIn;
    fade_timer_ = 0.f;

    is_dead_ = false;
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
        float alpha = (fade_state_ == FadeState::kFadeIn) ? t : (1.f - t);

        Math::Color color = renderer_->GetColor();
        color.a = static_cast<uint8_t>(alpha * 255.f);
        renderer_->SetColor(color);

        if (t >= 1.f)
        {
            if (fade_state_ == FadeState::kFadeOut)
            {
                SetActive(false);
            }
            
            fade_state_ = FadeState::kNone;
            fade_timer_ = 0.f;
        }
    }
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
