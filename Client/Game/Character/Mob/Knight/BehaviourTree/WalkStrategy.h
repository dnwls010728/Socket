#pragma once
#include "Character/BehaviourTree/IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class WalkStrategy : public IStrategy
    {
    public:
        WalkStrategy(AnimatorComponent* animator);
        ~WalkStrategy() = default;

        virtual Node::Status Process() override;

        virtual void Reset() override;

    private:
        AnimatorComponent* animator_;

        float start_time_;
    
    };
}
