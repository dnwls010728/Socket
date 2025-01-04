#pragma once
#include "Character/BehaviorTree/IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class WalkStrategy : public IStrategy
    {
    public:
        WalkStrategy(AnimatorComponent* animator);
        ~WalkStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        AnimatorComponent* animator_;
    
    };
}
