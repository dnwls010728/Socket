#include "pch.h"
#include "TimeOut.h"

BT::TimeOut::TimeOut(const std::wstring& kName, float duration) :
    Decorator(kName),
    is_started_(false),
    duration_(duration),
    timer_(0.f)
{
}

BT::Node::Status BT::TimeOut::TickNode(float delta_time)
{
    if (!is_started_)
    {
        if (!child_) return Status::kFailure;
        timer_ = duration_;
        is_started_ = true;
    }

    timer_ -= delta_time;
    if (timer_ <= 0.f)
    {
        Reset();
        return Status::kFailure;
    }

    Status status = child_->TickNode(delta_time);
    if (status == Status::kFailure) return Status::kFailure;
    if (status == Status::kSuccess) return Status::kSuccess;
    return Status::kRunning;
}

void BT::TimeOut::Reset()
{
    timer_ = 0.f;
    is_started_ = false;
    
    Decorator::Reset();
}

std::shared_ptr<BT::TimeOut> BT::TimeOut::Create(const std::wstring& kName, float duration)
{
    return std::make_shared<TimeOut>(kName, duration);
}
