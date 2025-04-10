#include "pch.h"
#include "Sequence.h"

BT::Sequence::Sequence(const std::wstring& kName) :
    Composite(kName)
{
}

BT::Node::Status BT::Sequence::TickNode(float delta_time)
{
    if (current_child_ < children_.size())
    {
        switch (children_[current_child_]->TickNode(delta_time))
        {
        case Status::kRunning: return Status::kRunning;
        case Status::kFailure:
            {
                Reset();
                return Status::kFailure;
            }

        default:
            {
                current_child_++;
                
                if (current_child_ == children_.size()) return Status::kSuccess;
                return Status::kRunning;
            }
        }
    }
    
    Reset();
    return Status::kSuccess;
}
