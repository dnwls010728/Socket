#include "pch.h"
#include "ServerObject.h"

#include <CustomPacket.h>

#include "DebugDrawHelper.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Actors/Characters/Components/Controller2DComponent.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/NameTag.h"
#include "UI/UIManager.h"
#include "Windows/DX/Sprite.h"
#include "GameInstance.h"
#include "Actor/Component/TransformComponent.h"
#include "Math/Math.h"

ServerObject::ServerObject(const std::wstring& kName) :
    NetworkActor(kName),
    state_machine_(nullptr),
    velocity_(Math::Vector2::Zero()),
    gravity_(-20.f)
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetSize({1.f, 1.f});

    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(std::numeric_limits<int32_t>::max());

    state_machine_ = AddComponent<StateMachineComponent>(L"StateMachine");

}


void ServerObject::BeginPlay()
{
    NetworkActor::BeginPlay();
}

void ServerObject::PhysicsTick(float delta_time)
{
    NetworkActor::PhysicsTick(delta_time);
    UpdateInterpolatedPosition(delta_time);
}

void ServerObject::Tick(float delta_time)
{
    NetworkActor::Tick(delta_time);

    
}

void ServerObject::UpdateInterpolatedPosition(float delta_time)
{
    float serverNow = GET_SESSION()->GetServerTime();

    // 현재 서버의 시간보다 interpolationDelay 지연된 시간
    float interpolationTime = serverNow - EngineSettings::Get()->GetInterpolationDelay();
    
    // 시간이 지난 스냅샷 제거
    while (snapshot_queue.size() >= 2 &&
           snapshot_queue[1].server_time < interpolationTime)
    {
        snapshot_queue.pop_front();
    }

    if (snapshot_queue.size() >= 2)
    {
        const auto& from = snapshot_queue[0];
        const auto& to   = snapshot_queue[1];

        float t = (interpolationTime - from.server_time) / (to.server_time - from.server_time);
        Math::Vector2 pos;
        pos.x = Math::Lerp(from.position.x, to.position.x, t);
        pos.y = Math::Lerp(from.position.y, to.position.y, t);
        
        GetTransform()->SetPosition(pos);
    }
}

void ServerObject::EndPlay(EndPlayReason type)
{
    NetworkActor::EndPlay(type);
}

void ServerObject::ReceivePacket(Net::IPacket* packet)
{
    NetworkActor::ReceivePacket(packet);

    switch (packet->GetPacketID())
    {
    case ObjectPositionPacket::StaticPacketID:
        {
            ObjectPositionPacket* position_packet = static_cast<ObjectPositionPacket*>(packet);
            Snapshot snapshot;
            snapshot.position = Math::Vector2(position_packet->position_x, position_packet->position_y);
            snapshot.velocity = Math::Vector2(position_packet->velocity_x, position_packet->velocity_y);
            snapshot.server_time = position_packet->server_time;
            snapshot_queue.push_back(snapshot);
        }
        break;
    default:
        break;
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ServerObject>("ServerObject")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
