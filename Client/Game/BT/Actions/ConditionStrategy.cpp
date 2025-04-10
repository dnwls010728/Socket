#include "pch.h"
#include "ConditionStrategy.h"

BT::ConditionStrategy::ConditionStrategy(bool(* func)()) :
    condition_(func)
{
}

BT::Node::Status BT::ConditionStrategy::TickNode(float delta_time)
{
    if (condition_()) return Node::Status::kSuccess;
    return Node::Status::kFailure;
}
