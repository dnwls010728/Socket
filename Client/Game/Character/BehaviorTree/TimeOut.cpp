#include "pch.h"
#include "TimeOut.h"

BT::TimeOut::TimeOut(const std::wstring& kName, float duration) :
    Node(kName),
    duration_(duration),
    timer_(0.f)
{
}

BT::Node::Status BT::TimeOut::TickNode(float delta_time)
{
    if (timer_ == 0.f) timer_ = duration_;
    
    timer_ -= delta_time;
    if (timer_ <= 0.f)
    {
        Reset();
        return Status::kFailure;
    }
    
    switch (children_[0]->TickNode(delta_time))
    {
    case Status::kSuccess:
        {
            return Status::kSuccess;
        }

    case Status::kFailure:
        {
            return Status::kFailure;
        }

    default:
        {
            return Status::kRunning;
        }
    }
}

void BT::TimeOut::Reset()
{
    Node::Reset();
    timer_ = 0.f;
}
