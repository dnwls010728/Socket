#include "pch.h"
#include "WaitForAnimationStrategy.h"

#include "Actor/Component/Animator/Animation.h"
#include "Actor/Component/Animator/AnimatorComponent.h"

BT::WaitForAnimationStrategy::WaitForAnimationStrategy(AnimatorComponent* animator, const std::wstring& kAnimationName) :
    animator_(animator),
    animation_name_(kAnimationName),
    is_init_(false)
{
}

BT::Node::Status BT::WaitForAnimationStrategy::TickNode(float delta_time)
{
    std::wstring a = animator_->GetCurrentAnimation()->GetName();
    
    if (!animator_) return Node::Status::kFailure;
    if (!is_init_)
    {
        is_init_ = true;
        animator_->PlayAnimation(animation_name_);
    }

    if (!animator_->IsPlaying()) return Node::Status::kSuccess;
    return Node::Status::kRunning;
}

void BT::WaitForAnimationStrategy::Reset()
{
    is_init_ = false;
}
