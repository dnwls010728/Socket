#include "pch.h"
#include "IdleStrategy.h"

#include "Logger.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Time/Time.h"

BT::IdleStrategy::IdleStrategy(AnimatorComponent* animator) :
    animator_(animator)
{
}

BT::Node::Status BT::IdleStrategy::Process()
{
    animator_->PlayAnimation(L"Idle");
    return Node::Status::kSuccess;
}

void BT::IdleStrategy::Reset()
{
}
