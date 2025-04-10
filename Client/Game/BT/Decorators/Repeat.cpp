#include "pch.h"
#include "Repeat.h"

BT::Repeat::Repeat(const std::wstring& kName, int count) :
    Decorator(kName),
    count_(count),
    counter_(0)
{
}

BT::Node::Status BT::Repeat::TickNode(float delta_time)
{
    if (!child_) return Status::kFailure;
    
    Status status = child_->TickNode(delta_time);
    if (status == Status::kFailure || status == Status::kSuccess)
    {
        if (count_ != 0 && ++counter_ >= count_)
        {
            Reset();
            return status;
        }
    }

    return Status::kRunning;
}

void BT::Repeat::Reset()
{
    counter_ = 0;
    
    Decorator::Reset();
}
