#pragma once
#include "Character/BehaviorTree/Actions/IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class WaitForAnimationStrategy : public IStrategy
    {
    public:
        WaitForAnimationStrategy(AnimatorComponent* animator, const std::wstring& kAnimationName);
        ~WaitForAnimationStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        AnimatorComponent* animator_;

        std::wstring animation_name_;

        bool is_init_;
    
    };
}
