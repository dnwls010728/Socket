#pragma once
#include "IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class SetAnimationFloatStrategy : public IStrategy
    {
    public:
        SetAnimationFloatStrategy(AnimatorComponent* animator, const std::wstring& kParameter, float value);
        ~SetAnimationFloatStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        AnimatorComponent* animator_;
        std::wstring parameter_;
        float value_;
        
    };
}
