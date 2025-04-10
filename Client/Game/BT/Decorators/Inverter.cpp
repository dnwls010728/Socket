#include "pch.h"
#include "Inverter.h"

BT::Inverter::Inverter(const std::wstring& kName) :
    Decorator(kName)
{
}

BT::Node::Status BT::Inverter::TickNode(float delta_time)
{
    if (!child_) return Status::kFailure;

    Status status = child_->TickNode(delta_time);
    if (status == Status::kFailure) return Status::kSuccess;
    if (status == Status::kSuccess) return Status::kFailure;
    return Status::kRunning;
}

std::shared_ptr<BT::Inverter> BT::Inverter::Create(const std::wstring& kName)
{
    return std::make_shared<Inverter>(kName);
}
