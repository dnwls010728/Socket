#include "pch.h"
#include "Inverter.h"

BT::Inverter::Inverter(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::Inverter::TickNode(float delta_time)
{
    switch (children_[0]->TickNode(delta_time))
    {
        case Status::kRunning:
            {
                return Status::kRunning;
            }

        case Status::kFailure:
            {
                return Status::kSuccess;
            }

        default:
            {
                return Status::kFailure;
            }
    }
}
