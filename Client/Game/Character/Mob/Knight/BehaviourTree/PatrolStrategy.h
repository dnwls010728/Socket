#pragma once
#include "Character/BehaviorTree/IStrategy.h"

class RigidBody2DComponent;
class AnimatorComponent;

namespace BT
{
    class PatrolStrategy : public IStrategy
    {
    public:
        PatrolStrategy(AnimatorComponent* animator, RigidBody2DComponent* rigid_body);
        ~PatrolStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        AnimatorComponent* animator_;
        RigidBody2DComponent* rigid_body_;

        float timer_;

        int direction_;
    
    };
}
