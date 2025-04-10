#include "pch.h"
#include "Selector.h"

BT::Selector::Selector(const std::wstring& kName) :
    Composite(kName)
{
}

BT::Node::Status BT::Selector::TickNode(float delta_time)
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

std::shared_ptr<BT::Selector> BT::Selector::Create(const std::wstring& kName)
{
    return std::make_shared<Selector>(kName);
}
