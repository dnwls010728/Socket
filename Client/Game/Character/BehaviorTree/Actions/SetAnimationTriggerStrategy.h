#pragma once
#include "IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class SetAnimationTriggerStrategy : public IStrategy
    {
    public:
        SetAnimationTriggerStrategy(AnimatorComponent* animator, const std::wstring& kParameter);
        ~SetAnimationTriggerStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        AnimatorComponent* animator_;
        std::wstring parameter_;
    
    };
}
