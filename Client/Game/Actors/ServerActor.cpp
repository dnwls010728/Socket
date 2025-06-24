#include "pch.h"
#include "ServerActor.h"

#include <CustomPacket.h>

#include "IPacket.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Subsystems/SessionSubsystem.h"

ServerActor::ServerActor(const std::wstring& name) :
    NetworkActor(name),
    snapshots_()
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(10000);
    
    animator_ = AddComponent<AnimatorComponent>(L"Animator");
}

void ServerActor::PhysicsTick(float delta_time)
{
    NetworkActor::PhysicsTick(delta_time);

    float server_now = SessionSubsystem::Get()->GetServerTime();

    float interpolation_time = server_now - EngineSettings::Get()->GetInterpolationDelay();

    while (snapshots_.size() >= 2 && snapshots_[1].server_time < interpolation_time)
    {
        snapshots_.pop_front();
    }

    if (snapshots_.size() >= 2)
    {
        const Snapshot& from = snapshots_[0];
        const Snapshot& to = snapshots_[1];

        float t = (interpolation_time - from.server_time) / (to.server_time - from.server_time);
        
        bool is_flipped = t < .5f ? from.is_flipped : to.is_flipped;
        std::wstring animation = t < .5f ? from.animation : to.animation;

        Math::Vector2 position = Math::Vector2::Lerp(from.position, to.position, t);
        GetTransform()->SetPosition(position);
        
        renderer_->SetFlipX(is_flipped);
        animator_->PlayAnimation(animation);
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

            Snapshot snapshot;
            snapshot.position.x = object_position_packet->position_x;
            snapshot.position.y = object_position_packet->position_y;
            snapshot.velocity.x = object_position_packet->velocity_x;
            snapshot.velocity.y = object_position_packet->velocity_y;
            snapshot.is_flipped = object_position_packet->is_flipped;
            snapshot.animation = object_position_packet->animation;
            snapshot.server_time = object_position_packet->server_time;
            snapshots_.push_back(snapshot);
        }
        break;
    }
}
