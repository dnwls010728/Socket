#include "pch.h"
#include "Root.h"

BT::Root::Root(const std::wstring& kName) :
    Node(kName),
    is_repeat_(true)
{
}

BT::Node::Status BT::Root::TickNode(float delta_time)
{
    if (children_.empty()) return Status::kSuccess;
    
    Status status = children_[0]->TickNode(delta_time);
    if (status == Status::kFailure || status == Status::kSuccess)
    {
        if (!is_repeat_) return status;
    }
    
    return Status::kRunning;
}
