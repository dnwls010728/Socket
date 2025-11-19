#include "pch.h"
#include "Projectile.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/NetworkSubsystem.h"

Projectile::Projectile(const std::wstring& name) :
    ServerActor(name),
    projectile_id_(0)
{
    SetLayer(ActorLayer::kMob);

    collider_->SetOffset({ 0.f, 0.f });
    collider_->SetSize({ 1.f, 1.f });
}

void Projectile::Init(uint32_t projectile_id)
{
    projectile_id_ = projectile_id;

    const ProjectileData* data = DataSubsystem::Get()->GetProjectile(projectile_id);
    if (!data) return;

    if (!data->animation_pack.empty())
    {
        AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(data->animation_pack);
        if (animation_pack) animator_->SetAnimationPack(animation_pack);
    }

    collider_->SetSize(data->size);
    animator_->PlayAnimation(data->animation);
}

void Projectile::OnActivate()
{
    if (HasBegunPlay()) SetActive(true);
}

void Projectile::OnDeactivate()
{
    SetActive(false);
}

void Projectile::OnEnable()
{
    ServerActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
}

void Projectile::OnDisable()
{
    ServerActor::OnDisable();
    NetworkSubsystem::Get()->UnregisterNetworkActor(GetSharedThis());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Projectile>("Projectile")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}

