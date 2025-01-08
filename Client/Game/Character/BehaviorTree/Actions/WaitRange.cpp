#include "pch.h"
#include "WaitRange.h"

#include "Math/Math.h"

BT::WaitRange::WaitRange(const std::wstring& kName, float min, float max) :
    Node(kName),
    min_(min),
    max_(max),
    timer_(0.f)
{
}

BT::Node::Status BT::WaitRange::TickNode(float delta_time)
{
    if (timer_ == 0.f) timer_ = Math::RandRange(min_, max_);
    
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
}
