#include "pch.h"
#include "ServerActor.h"

#include <CustomPacket.h>

#include "Damage.h"
#include "IPacket.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Math/Math.h"
#include "Subsystems/SessionSubsystem.h"

ServerActor::ServerActor(const std::wstring& name) :
    NetworkActor(name),
    prev_animation{0,},
    last_damage_spawn_time_(0.f)
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(10000);
    
    animator_ = AddComponent<AnimatorComponent>(L"Animator");
}

void ServerActor::SetFlip(bool is_fliped)
{
    renderer_->SetFlipX(is_fliped);
}

void ServerActor::PlayAnimation(const std::wstring& animation)
{
    animation_snapshots_.clear();
    
    AnimationSnapshot snapshot;
    snapshot.animation = animation;
    snapshot.is_flipped = renderer_->IsFlipX();
    snapshot.server_time = Net::GetClientTime();
    animation_snapshots_.push_back(snapshot);
}

void ServerActor::TakeDamage(std::vector<int> damage_amount)
{
    for (int i = 0; i < damage_amount.size(); ++i)
    {
        DamageSnapshot snapshot;
        snapshot.damage_amount = damage_amount[i];
        snapshot.position = GetTransform()->GetPosition() + Math::Vector2::Up() * 2.f;
        snapshot.position.y += i * 0.5f;
        pending_damages_.push_back(snapshot);
    }
}

void ServerActor::PhysicsTick(float delta_time)
{
    NetworkActor::PhysicsTick(delta_time);
    float server_now = SessionSubsystem::Get()->GetServerTime();
    float interpolation_time = server_now - EngineSettings::Get()->GetObjectInterpolationDelay();

    // 오래된 스냅샷 제거
    while (movement_snapshots_.size() >= 2 &&
        movement_snapshots_[1].server_time < interpolation_time)
    {
        movement_snapshots_.pop_front(); 
    }

    if (movement_snapshots_.size() >= 2 &&
        movement_snapshots_[1].time_update)
    {
        movement_snapshots_.pop_front();
    }
        
    if (movement_snapshots_.size() >= 2)
    {
        if (movement_snapshots_[0].time_update)
        {
            movement_snapshots_[0].server_time = movement_snapshots_[1].server_time - 0.15f;
        }
            
        const MovementSnapshot& from = movement_snapshots_[0];
        const MovementSnapshot& to = movement_snapshots_[1];

        float t = (interpolation_time - from.server_time) / (to.server_time - from.server_time);
        t = Math::Clamp(t, 0.f, 1.f);

        Math::Vector2 position = Math::Vector2::Lerp(from.position, to.position, t);
        GetTransform()->SetPosition(position);
    }
    
    while (animation_snapshots_.size() >= 2 &&
       animation_snapshots_[1].server_time < interpolation_time)
    {
        animation_snapshots_.pop_front();
    }

    if (!animation_snapshots_.empty())
    {
        const auto& anim = animation_snapshots_.front();

        // 이전 스냅샷과 다를 경우에만 처리
        if (prev_animation.server_time != anim.server_time)
        {
            renderer_->SetFlipX(anim.is_flipped);
            animator_->PlayAnimation(anim.animation);
            prev_animation = anim;
        }
    }
}

void ServerActor::Tick(float delta_time)
{
    NetworkActor::Tick(delta_time);

    float now = Net::GetClientTime();

    if (!pending_damages_.empty())
    {
        if (now - last_damage_spawn_time_ >= 0.1f)
        {
            const auto& damage_snapshot = pending_damages_.front();
            OnShowDamage(damage_snapshot);
            pending_damages_.pop_front();
        }
    }
}

void ServerActor::ReceivePacket(Net::IPacket* packet)
{
    NetworkActor::ReceivePacket(packet);

    switch (packet->GetPacketID())
    {
    case ObjectPositionPacket::StaticPacketID:
        {
            ObjectPositionPacket* object_position_packet = static_cast<ObjectPositionPacket*>(packet);

            MovementSnapshot snapshot;
            snapshot.position.x = object_position_packet->position_x;
            snapshot.position.y = object_position_packet->position_y;
            snapshot.velocity.x = object_position_packet->velocity_x;
            snapshot.velocity.y = object_position_packet->velocity_y;
            snapshot.server_time = object_position_packet->server_time;
            snapshot.time_update =  object_position_packet->time_update;
            movement_snapshots_.push_back(snapshot);
        }
        break;

    case ObjectAnimationPacket::StaticPacketID:
        {
            ObjectAnimationPacket* object_position_packet = static_cast<ObjectAnimationPacket*>(packet);

            if (object_position_packet->instant_play)
                animation_snapshots_.clear();
            AnimationSnapshot snapshot;
            snapshot.animation = object_position_packet->animation;
            snapshot.is_flipped = object_position_packet->is_flipped;
            snapshot.server_time = object_position_packet->server_time;
            animation_snapshots_.push_back(snapshot);
        }
        break;
    }
}

void ServerActor::OnShowDamage(const DamageSnapshot& damage_snapshot)
{
    float now = Net::GetClientTime();
    std::shared_ptr<Damage> damage = World::Get()->SpawnActor<Damage>(Damage::StaticClass());
    if (IsValid(damage))
    {
        damage->SetDamage(damage_snapshot.damage_amount);
        damage->GetTransform()->SetPosition(damage_snapshot.position);
        last_damage_spawn_time_ = now;
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ServerActor>("ServerActor")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
