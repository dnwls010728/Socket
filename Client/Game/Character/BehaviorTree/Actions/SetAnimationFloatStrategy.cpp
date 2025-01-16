#include "pch.h"
#include "SetAnimationFloatStrategy.h"

#include "Actor/Component/Animator/AnimatorComponent.h"

BT::SetAnimationFloatStrategy::SetAnimationFloatStrategy(AnimatorComponent* animator, const std::wstring& kParameter, float value) :
    animator_(animator),
    parameter_(kParameter),
    value_(value)
{
}

BT::Node::Status BT::SetAnimationFloatStrategy::TickNode(float delta_time)
{
    if (!animator_) return Node::Status::kFailure;

    animator_->SetFloat(parameter_, value_);
    return Node::Status::kSuccess;
}
