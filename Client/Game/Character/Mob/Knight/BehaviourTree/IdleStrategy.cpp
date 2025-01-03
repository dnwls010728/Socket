#include "pch.h"
#include "IdleStrategy.h"

#include "Logger.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Time/Time.h"

BT::IdleStrategy::IdleStrategy(AnimatorComponent* animator) :
    animator_(animator),
    start_time_(0.f)
{
}

BT::Node::Status BT::IdleStrategy::Process()
{
    if (start_time_ == 0.f)
    {
        animator_->PlayAnimation(L"Idle");
        start_time_ = Time::Seconds();
    }
    
    if (Time::Seconds() - start_time_ > 1.f)
    {
        return Node::Status::kSuccess;
    }
    
    return Node::Status::kRunning;
}

void BT::IdleStrategy::Reset()
{
    start_time_ = 0.f;
}
