#include "pch.h"
#include "Sequence.h"

BT::Sequence::Sequence(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::Sequence::Process()
{
    if (current_child_ < children_.size())
    {
        switch (children_[current_child_]->Process())
        {
        case Status::kRunning:
            {
                return Status::kRunning;
            }
            
        case Status::kFailure:
            {
                Reset();
                return Status::kFailure;
            }
            
        default:
            {
                current_child_++;
                return current_child_ == children_.size() ? Status::kSuccess : Status::kRunning;
            }
        }
    }

    Reset();
    return Status::kSuccess;
}
