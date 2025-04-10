#include "pch.h"
#include "Selector.h"

BT::Selector::Selector(const std::wstring& kName) :
    Composite(kName)
{
}

BT::Node::Status BT::Selector::Tick(float delta_time)
{
    if (current_child_ < children_.size())
    {
        switch (children_[current_child_]->TickNode(delta_time))
        {
        case Status::kRunning: return Status::kRunning;
        case Status::kSuccess:
            {
                Reset();
                return Status::kSuccess;
            }

        default:
            {
                current_child_++;
                return Status::kRunning;
            }
        }
    }
    
    Reset();
    return Status::kFailure;
}
