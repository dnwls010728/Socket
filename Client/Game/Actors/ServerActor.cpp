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
    float server_now = SessionSubsystem::Get()->GetServerTime();
    float interpolation_time = server_now - EngineSettings::Get()->GetInterpolationDelay();

    while (snapshots_.size() >= 2 && snapshots_[1].server_time < interpolation_time)
    {
        snapshots_.pop_front();
    }
    
    if (snapshots_.size() >= 2 && snapshots_[1].time_update)
    {
        snapshots_.pop_front();
    }

    if (snapshots_.size() >= 2 && snapshots_[0].time_update)
    {
        snapshots_[0].server_time = snapshots_[1].server_time - EngineSettings::Get()->GetInterpolationDelay();
    }

    if (snapshots_.size() >= 2)
    {
        const Snapshot& from = snapshots_[0];
        const Snapshot& to = snapshots_[1];

        float t = (interpolation_time - from.server_time) / (to.server_time - from.server_time);
        t = Math::Clamp(t, 0.f, 1.f);
        
        Math::Vector2 position = Math::Vector2::Lerp(from.position, to.position, t);
        GetTransform()->SetPosition(position);
        
        uint16_t packed_state = t < .5f ? from.state : to.state;
        
        uint8_t state = (packed_state & 0xFF00) >> 8;
        bool is_flipped = (packed_state & 0x0001) != 0;

        OnState(state, is_flipped);
    }
    /*
     *    else if (snapshots_.size() == 1) {
        const Snapshot& s = snapshots_[0];
        float delta = interpolation_time - s.server_time;
        // delta 클램프 대신, 움직임 강도에 따라 늘리거나 줄인다
        float maxDelta = std::max(0.1f, std::min(delta, 0.5f));
        Math::Vector2 pos = s.position + s.velocity * maxDelta;
        // 애니메이션·플립도 바로 적용
        GetTransform()->SetPosition(pos);
        renderer_->SetFlipX(s.is_flipped);
        animator_->PlayAnimation(s.animation);
    }*/
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
            snapshot.state = object_position_packet->state;
            snapshot.server_time = object_position_packet->server_time;
            snapshot.time_update =  object_position_packet->time_update;
            snapshots_.push_back(snapshot);
        }
        break;
    }
}

void ServerActor::OnState(uint8_t state, bool is_flipped)
{
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
