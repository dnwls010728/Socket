#include "pch.h"
#include "WalkStrategy.h"

#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Time/Time.h"

BT::WalkStrategy::WalkStrategy(AnimatorComponent* animator) :
    animator_(animator)
{
}

BT::Node::Status BT::WalkStrategy::Process()
{
    animator_->PlayAnimation(L"Run");
    return Node::Status::kSuccess;
}

void BT::WalkStrategy::Reset()
{
}
