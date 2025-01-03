#pragma once
#include "Character/BehaviourTree/IStrategy.h"

namespace BT
{
    class WanderStrategy : public IStrategy
    {
    public:
        WanderStrategy();

        virtual Node::Status Process() override;
    
    };
}
