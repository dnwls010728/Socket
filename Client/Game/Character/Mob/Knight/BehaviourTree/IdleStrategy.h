#pragma once
#include "Character/BehaviourTree/IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class IdleStrategy : public IStrategy
    {
    public:
        IdleStrategy(AnimatorComponent* animator);
        ~IdleStrategy() = default;

        virtual Node::Status Process() override;
        
        virtual void Reset() override;

    private:
        AnimatorComponent* animator_;
    
    };
}
