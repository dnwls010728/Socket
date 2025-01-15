#include "pch.h"
#include "SetAnimationIntStrategy.h"

#include "Actor/Component/Animator/AnimatorComponent.h"

BT::SetAnimationIntStrategy::SetAnimationIntStrategy(AnimatorComponent* animator, const std::wstring& kParameter, int value) :
    animator_(animator),
    parameter_(kParameter),
    value_(value)
{
}

BT::Node::Status BT::SetAnimationIntStrategy::TickNode(float delta_time)
{
    if (!animator_) return Node::Status::kFailure;

    animator_->SetInt(parameter_, value_);
    return Node::Status::kSuccess;
}
