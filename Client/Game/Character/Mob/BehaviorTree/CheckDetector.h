#pragma once
#include "Character/BehaviorTree/IStrategy.h"

namespace Blackboard
{
    class Blackboard;
}

namespace BT
{
    class CheckDetector : public IStrategy
    {
    public:
        CheckDetector(Blackboard::Blackboard* blackboard);
        ~CheckDetector() = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        Blackboard::Blackboard* blackboard_;
    
    };
}
