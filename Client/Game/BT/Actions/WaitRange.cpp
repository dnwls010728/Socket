#include "pch.h"
#include "WaitRange.h"

#include "Math/Math.h"

BT::WaitRange::WaitRange(const std::wstring& kName, float min, float max) :
    Node(kName),
    min_(min),
    max_(max),
    timer_(0.f),
    is_started_(false)
{
}

BT::Node::Status BT::WaitRange::TickNode(float delta_time)
{
    if (!is_started_)
    {
        timer_ = Math::RandRange(min_, max_);
        is_started_ = true;
    }

    timer_ -= delta_time;
    if (timer_ <= 0.f)
    {
        Reset();
        return Status::kSuccess;
    }

    return Status::kRunning;
}

void BT::WaitRange::Reset()
{
    timer_ = 0.f;
    is_started_ = false;
    
    Node::Reset();
}
