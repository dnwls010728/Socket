#include "pch.h"
#include "ServerActor.h"

#include <CustomPacket.h>

#include "IPacket.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Math/Math.h"
#include "Subsystems/SessionSubsystem.h"

ServerActor::ServerActor(const std::wstring& name) :
    NetworkActor(name)
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(10000);
    
    animator_ = AddComponent<AnimatorComponent>(L"Animator");
}

void ServerActor::PlayPredictedAnimation(std::wstring animation)
{
    animation_snapshots_.clear();
    animator_->PlayAnimation(animation);
}

void ServerActor::PhysicsTick(float delta_time)
{
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
        renderer_->SetFlipX(anim.is_flipped);
        animator_->PlayAnimation(anim.animation);
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

            AnimationSnapshot snapshot;
            snapshot.animation = object_position_packet->animation;
            snapshot.is_flipped = object_position_packet->is_flipped;
            snapshot.server_time = object_position_packet->server_time;
            animation_snapshots_.push_back(snapshot);
        }
        break;
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
