#pragma once
#include "Character/BehaviorTree/Actions/IStrategy.h"
#include "Character/Blackboard/BlackboardKey.h"
#include "Math/Vector2.h"

class AnimatorComponent;
class ContextSteeringComponent;
class Actor;

namespace Blackboard
{
    class Blackboard;
}

namespace BT
{
    class MoveToTargetStrategy : public IStrategy
    {
    public:
        MoveToTargetStrategy(Blackboard::Blackboard* blackboard);
        ~MoveToTargetStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        Blackboard::Blackboard* blackboard_;
        
        Node::Status Validate();
        
        Math::Vector2 GetPositionColliderAdjusted() const;

        float GetDistance();

        bool is_init_;

        Blackboard::BlackboardKey self_key_;
        Blackboard::BlackboardKey target_key_;

        Actor* self_;
        Actor* target_;

        Math::Vector2 last_position_;
        Math::Vector2 collider_adjusted_position_;

        float distance_threshold_;
        float collider_offset_;
        float previous_stopping_distance_;

        AnimatorComponent* animator_;
        ContextSteeringComponent* context_steering_;
    
    };
}
