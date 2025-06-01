#include "pch.h"
#include "ServerActor.h"

#include <CustomPacket.h>

#include "IPacket.h"
#include "Actor/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Subsystems/SessionSubsystem.h"

ServerActor::ServerActor(const std::wstring& name) :
    NetworkActor(name),
    snapshots_()
{
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

        Math::Vector2 position;
        position.x = Math::Lerp(from.position.x, to.position.x, t);
        position.y = Math::Lerp(from.position.y, to.position.y, t);

        GetTransform()->SetPosition(position);
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
            snapshot.server_time = object_position_packet->server_time;
            snapshots_.push_back(snapshot);
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
