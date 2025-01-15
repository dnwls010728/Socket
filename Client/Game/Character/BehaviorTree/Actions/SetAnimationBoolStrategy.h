#pragma once
#include "IStrategy.h"

class AnimatorComponent;

namespace BT
{
    class SetAnimationBoolStrategy : public IStrategy
    {
    public:
        SetAnimationBoolStrategy(AnimatorComponent* animator, const std::wstring& kParameter, bool value);
        ~SetAnimationBoolStrategy() = default;

        virtual Node::Status TickNode(float delta_time) override;

    private:
        AnimatorComponent* animator_;
        std::wstring parameter_;
        bool value_;
    
    };
}
