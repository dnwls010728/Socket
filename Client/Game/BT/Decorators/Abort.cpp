#include "pch.h"
#include "Abort.h"

BT::Abort::Abort(const std::wstring& kName, bool(* func)()) :
    Decorator(kName),
    condition_(func)
{
}

BT::Node::Status BT::Abort::TickNode(float delta_time)
{
    if (!child_) return Status::kFailure;
    if (condition_())
    {
        Reset();
        return Status::kFailure;
    }

    Status status = child_->TickNode(delta_time);
    if (status == Status::kFailure) return Status::kFailure;
    if (status == Status::kSuccess) return Status::kSuccess;
    return Status::kRunning;
}
