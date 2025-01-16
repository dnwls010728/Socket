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
    class MoveToLocationStrategy : public IStrategy
    {
    public:
        MoveToLocationStrategy(Blackboard::Blackboard* blackboard);
        ~MoveToLocationStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        Blackboard::Blackboard* blackboard_;
        
        Node::Status Validate();

        float GetDistanceToLcoation(Math::Vector2& agent_position, Math::Vector2& location_position);

        bool is_init_;

        Blackboard::BlackboardKey self_key_;
        Blackboard::BlackboardKey location_key_;

        Actor* self_;

        Math::Vector2 destination_;

        float distance_threshold_;
        float previous_stopping_distance_;

        AnimatorComponent* animator_;
        ContextSteeringComponent* context_steering_;
    
    };
}
