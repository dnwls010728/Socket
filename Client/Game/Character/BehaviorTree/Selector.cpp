#include "pch.h"
#include "Selector.h"

BT::Selector::Selector(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::Selector::Process()
{
    if (current_child_ < children_.size())
    {
        switch (children_[current_child_]->Process())
        {
        case Status::kRunning:
            {
                return Status::kRunning;
            }

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
