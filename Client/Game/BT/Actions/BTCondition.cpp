#include "pch.h"
#include "BTCondition.h"

BT::Condition::Condition(bool(* func)()) :
    condition_(func)
{
}

BT::Node::Status BT::Condition::TickNode(float delta_time)
{
    if (condition_()) return Node::Status::kSuccess;
    return Node::Status::kFailure;
}
