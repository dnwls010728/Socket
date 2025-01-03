#include "pch.h"
#include "Inverter.h"

BT::Inverter::Inverter(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::Inverter::Process()
{
    switch (children_[0]->Process())
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
