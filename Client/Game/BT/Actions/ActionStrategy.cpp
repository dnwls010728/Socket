#include "pch.h"
#include "ActionStrategy.h"

BT::ActionStrategy::ActionStrategy(void(* func)()) :
    action_(func)
{
}

BT::Node::Status BT::ActionStrategy::TickNode(float delta_time)
{
    action_();
    return Node::Status::kSuccess;
}

std::shared_ptr<BT::ActionStrategy> BT::ActionStrategy::Create(void(* func)())
{
    return std::make_shared<ActionStrategy>(func);
}
