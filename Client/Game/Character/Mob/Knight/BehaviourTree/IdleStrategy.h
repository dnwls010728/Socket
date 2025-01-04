#pragma once
#include "Character/BehaviorTree/IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class IdleStrategy : public IStrategy
    {
    public:
        IdleStrategy(AnimatorComponent* animator);
        ~IdleStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;
        
        virtual void Reset() override;

    private:
        AnimatorComponent* animator_;
    
    };
}
