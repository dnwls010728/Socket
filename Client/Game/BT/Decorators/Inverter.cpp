#include "pch.h"
#include "Inverter.h"

BT::Node::Status BT::Inverter::TickNode(float delta_time)
{
    if (!child_) return Status::kFailure;

    Status status = child_->TickNode(delta_time);
    if (status == Status::kFailure) return Status::kSuccess;
    if (status == Status::kSuccess) return Status::kFailure;
    return Status::kRunning;
}
