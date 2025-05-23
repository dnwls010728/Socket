#pragma once
#include "Actors/NetworkActor.h"
#include <deque>

class Controller2DComponent;
class StateMachineComponent;

class ServerObject : public NetworkActor
{
    SHADER_CLASS_HELPER(ServerObject)
    GENERATED_BODY(ServerObject, NetworkActor)

    struct Snapshot
    {
        float server_time;
        Math::Vector2 position;
        Math::Vector2 velocity;
    };
public:
    ServerObject(const std::wstring& kName);
    virtual ~ServerObject() override = default;
    
    FORCEINLINE std::shared_ptr<SpriteRendererComponent> GetRenderer() const { return renderer_; }
    FORCEINLINE std::shared_ptr<StateMachineComponent> GetStateMachine() const { return state_machine_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;

    virtual void ReceivePacket(Net::IPacket* packet);

    virtual void UpdateInterpolatedPosition(float delta_time);
#pragma region 컴포넌트
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<StateMachineComponent> state_machine_;
#pragma endregion

    Math::Vector2 velocity_;
    float gravity_;
    std::deque<Snapshot> snapshot_queue;
    
};
