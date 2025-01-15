#include "pch.h"
#include "SetAnimationTriggerStrategy.h"

#include "Actor/Component/Animator/AnimatorComponent.h"

BT::SetAnimationTriggerStrategy::SetAnimationTriggerStrategy(AnimatorComponent* animator, const std::wstring& kParameter) :
    animator_(animator),
    parameter_(kParameter)
{
}

BT::Node::Status BT::SetAnimationTriggerStrategy::TickNode(float delta_time)
{
    if (!animator_) return Node::Status::kFailure;

    animator_->SetTrigger(parameter_);
    return Node::Status::kSuccess;
}
