#include "pch.h"
#include "MobBase.h"

#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Subsystems/NetworkSubsystem.h"

MobBase::MobBase(const std::wstring& name) :
    ServerActor(name),
    is_dead_(false),
    fade_timer_(0.f)
{
    SetLayer(ActorLayer::kMob);

    // 임시
    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Mobs\\MushroomSheet.png.animpack");
    if (animation_pack) animator_->SetAnimationPack(animation_pack);
    
}

void MobBase::OnActivate()
{
    SetActive(true);
}

void MobBase::OnDeactivate()
{
    SetActive(false);
}

void MobBase::BeginPlay()
{
    ServerActor::BeginPlay();

    animator_->PlayAnimation(L"Idle");
}

void MobBase::OnEnable()
{
    ServerActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
}

void MobBase::OnDisable()
{
    ServerActor::OnDisable();
    NetworkSubsystem::Get()->UnregisterNetworkActor(GetSharedThis());
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
