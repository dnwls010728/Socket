#include "pch.h"
#include "Start.h"

BT::Start::Start(const std::wstring& kName) :
    Decorator(kName),
    is_repeat_(true)
{
}

BT::Node::Status BT::Start::TickNode(float delta_time)
{
    if (!child_) return Status::kSuccess;

    Status status = child_->TickNode(delta_time);
    if (status == Status::kFailure || status == Status::kSuccess)
    {
        if (!is_repeat_) return status;
    }

    return Status::kRunning;
}

std::shared_ptr<BT::Start> BT::Start::Create(const std::wstring& kName)
{
    return std::make_shared<Start>(kName);
}
