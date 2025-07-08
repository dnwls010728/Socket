#pragma once
#include "NetworkActor.h"

class ServerActor : public NetworkActor
{
    SHADER_CLASS_HELPER(ServerActor)
    GENERATED_BODY(ServerActor, NetworkActor)
    
public:
    struct MovementSnapshot
    {
        Math::Vector2 position;
        Math::Vector2 velocity;
        
        float server_time;

        bool time_update;
    };

    struct AnimationSnapshot
    {
        bool          is_flipped;
        std::wstring  animation;
        float         server_time;
    };
    
    ServerActor(const std::wstring& name);
    virtual ~ServerActor() override = default;

    void PlayPredictedAnimation(std::wstring animation);

protected:
    virtual void PhysicsTick(float delta_time) override;
    virtual void ReceivePacket(Net::IPacket* packet) override;
    
    std::deque<MovementSnapshot>  movement_snapshots_;
public:
    std::deque<AnimationSnapshot> animation_snapshots_;
    
#pragma region 컴포넌트
    std::shared_ptr<class BoxColliderComponent> collider_;
    std::shared_ptr<class SpriteRendererComponent> renderer_;
    std::shared_ptr<class AnimatorComponent> animator_;
#pragma endregion
    
};
