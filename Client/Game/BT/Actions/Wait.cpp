#include "pch.h"
#include "Wait.h"

BT::Wait::Wait(const std::wstring& kName, float duration) :
    Node(kName),
    duration_(duration),
    timer_(0.f),
    is_started_(false)
{
}

BT::Node::Status BT::Wait::TickNode(float delta_time)
{
    if (!is_started_)
    {
        timer_ = duration_;
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

void BT::Wait::Reset()
{
    timer_ = 0.f;
    is_started_ = false;
    
    Node::Reset();
}

std::shared_ptr<BT::Wait> BT::Wait::Create(const std::wstring& kName, float duration)
{
    return std::make_shared<Wait>(kName, duration);
}
