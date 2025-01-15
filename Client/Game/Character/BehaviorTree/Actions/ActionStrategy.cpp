#include "pch.h"
#include "ActionStrategy.h"

BT::ActionStrategy::ActionStrategy(void(* func)()) :
    func_(func)
{
}

BT::Node::Status BT::ActionStrategy::TickNode(float delta_time)
{
    func_();
    return Node::Status::kSuccess;
}
