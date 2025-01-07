#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Math/Vector2.h"

class ContextSteering : public ActorComponent
{
    SHADER_CLASS_HELPER(ContextSteering)
    GENERATED_BODY(ContextSteering, ActorComponent)
    
public:
    ContextSteering(Actor* owner, const std::wstring& kName);
    virtual ~ContextSteering() override = default;

    void SetDestination(const Math::Vector2& destination);
    void Stop();

    bool IsComplete();

    FORCEINLINE void SetSpeed(float speed) { speed_ = speed; }
    FORCEINLINE void SetStoppingDistance(float distance) { stopping_distance_ = distance; }

    FORCEINLINE float GetSpeed() const { return speed_; }
    FORCEINLINE float GetStoppingDistance() const { return stopping_distance_; }

    FORCEINLINE bool IsStopped() const { return is_stopped_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTickComponent(float delta_time) override;
    virtual void TickComponent(float delta_time) override;

private:
    void DetectObstacles();
    void GetDangerSteering(float (&danger)[8]);
    void GetSeekSteering(float (&interest)[8]);
    
    Math::Vector2 GetDirectionToMove();

    class RigidBody2DComponent* rigid_body_;

    float speed_;
    float stopping_distance_;
    
    bool is_stopped_;
    
    Math::Vector2 destination_;
    Math::Vector2 direction_;

    std::vector<Actor*> obstacles_;

    ActorLayer obstacle_layer_;

    static std::vector<Math::Vector2> directions_;
    
};
