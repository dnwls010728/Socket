#include "pch.h"
#include "SetAnimationBoolStrategy.h"

#include "Actor/Component/Animator/AnimatorComponent.h"

BT::SetAnimationBoolStrategy::SetAnimationBoolStrategy(AnimatorComponent* animator, const std::wstring& kParameter, bool value) :
    animator_(animator),
    parameter_(kParameter),
    value_(value)
{
}

BT::Node::Status BT::SetAnimationBoolStrategy::TickNode(float delta_time)
{
    if (!animator_) return Node::Status::kFailure;

    animator_->SetBool(parameter_, value_);
    return Node::Status::kSuccess;
}
