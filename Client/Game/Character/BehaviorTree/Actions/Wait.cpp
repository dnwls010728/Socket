#include "pch.h"
#include "Wait.h"

#include "Time/Time.h"

BT::Wait::Wait(const std::wstring& kName, float duration) :
    Node(kName),
    duration_(duration),
    timer_(0.f)
{
}

BT::Node::Status BT::Wait::TickNode(float delta_time)
{
    if (timer_ == 0.f) timer_ = duration_;
    
    timer_ -= delta_time;
    if (timer_ <= 0.f)
    {
        Reset();
        return Status::kSuccess;
    }

    return Status::kRunning;
}

void BT::Wait::Reset()
{
    timer_ = 0.f;
}
