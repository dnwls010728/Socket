#include "pch.h"
#include "MobBase.h"

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
    is_fade_in_(false),
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
    SetActive(true);
}

void MobBase::OnDeactivate()
{
    SetActive(false);
}

void MobBase::OnEnable()
{
    ServerActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
    
    renderer_->SetFlipX(false);
    animator_->PlayAnimation(L"Idle");

    fade_timer_ = 0.f;
    is_fade_in_ = true;
}

void MobBase::OnDisable()
{
    ServerActor::OnDisable();
    NetworkSubsystem::Get()->UnregisterNetworkActor(GetSharedThis());
}

void MobBase::Tick(float delta_time)
{
    ServerActor::Tick(delta_time);

    if (is_fade_in_)
    {
        Math::Color color = renderer_->GetColor();
        if (fade_timer_ < 2.f)
        {
            color.a = Math::Lerp(0.f, 1.f, fade_timer_ / 2.f) * 255.f;
            fade_timer_ += delta_time;
        }

        renderer_->SetColor(color);
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
