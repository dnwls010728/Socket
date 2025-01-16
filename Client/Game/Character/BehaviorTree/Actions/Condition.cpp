#include "pch.h"
#include "Condition.h"

BT::Condition::Condition(bool(* func)()) :
    func_(func)
{
}

BT::Node::Status BT::Condition::TickNode(float delta_time)
{
    return func_() ? Node::Status::kSuccess : Node::Status::kFailure;
}
