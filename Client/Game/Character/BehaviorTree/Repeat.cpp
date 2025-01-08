#include "pch.h"
#include "Repeat.h"

BT::Repeat::Repeat(const std::wstring& kName, int count) :
    Node(kName),
    count_(count),
    current_count_(0)
{
}

BT::Node::Status BT::Repeat::TickNode(float delta_time)
{
    Status status = children_[0]->TickNode(delta_time);
    if (status == Status::kFailure || status == Status::kSuccess)
    {
        if (count_ != 0 && ++current_count_ >= count_)
        {
            Reset();
            return status;
        }
    }

    return Status::kRunning;
}

void BT::Repeat::Reset()
{
    current_count_ = 0;
}
