#include "pch.h"
#include "UntilFall.h"

BT::UntilFall::UntilFall(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::UntilFall::Process()
{
    if (children_[0]->Process() == Status::kFailure)
    {
        Reset();
        return Status::kFailure;
    }

    return Status::kRunning;
}
