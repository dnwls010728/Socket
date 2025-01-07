#pragma once
#include "Character/BehaviorTree/IStrategy.h"

namespace Blackboard
{
    class Blackboard;
}

namespace BT
{
    class CheckDetectorStrategy : public IStrategy
    {
    public:
        CheckDetectorStrategy(Blackboard::Blackboard* blackboard);
        ~CheckDetectorStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        Blackboard::Blackboard* blackboard_;
    
    };
}
