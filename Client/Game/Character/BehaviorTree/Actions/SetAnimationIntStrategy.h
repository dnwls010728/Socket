#pragma once
#include "IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class SetAnimationIntStrategy : public IStrategy
    {
    public:
        SetAnimationIntStrategy(AnimatorComponent* animator, const std::wstring& kParameter, int value);
        ~SetAnimationIntStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        AnimatorComponent* animator_;
        std::wstring parameter_;
        int value_;
    
    };
}
