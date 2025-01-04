#include "pch.h"
#include "Wait.h"

#include "Time/Time.h"

BT::Wait::Wait(const std::wstring& kName, float duration) :
    Node(kName),
    duration_(duration),
    start_time_(0.f)
{
}

BT::Node::Status BT::Wait::TickNode(float delta_time)
{
    if (start_time_ == 0.f) start_time_ = Time::Seconds();
    if (Time::Seconds() - start_time_ > duration_) return Status::kSuccess;
    return Status::kRunning;
}

void BT::Wait::Reset()
{
    start_time_ = 0.f;
}
