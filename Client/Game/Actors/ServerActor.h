#pragma once
#include "NetworkActor.h"

class ServerActor : public NetworkActor
{
    SHADER_CLASS_HELPER(ServerActor)
    GENERATED_BODY(ServerActor, NetworkActor)
    
public:
    struct Snapshot
    {
        Math::Vector2 position;
        Math::Vector2 velocity;
        
        bool is_flipped;
        
        std::wstring animation;
        
        float server_time;

        bool time_update;
    };
    
    ServerActor(const std::wstring& name);
    virtual ~ServerActor() override = default;

protected:
    virtual void PhysicsTick(float delta_time) override;
    virtual void ReceivePacket(Net::IPacket* packet) override;

    std::deque<Snapshot> snapshots_;

    float animation_changed_time_;

    std::wstring last_animation_;
    
#pragma region 컴포넌트
    std::shared_ptr<class BoxColliderComponent> collider_;
    std::shared_ptr<class SpriteRendererComponent> renderer_;
    std::shared_ptr<class AnimatorComponent> animator_;
#pragma endregion
    
};
