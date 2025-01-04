#include "pch.h"
#include "Condition.h"

BT::Condition::Condition(const Function<bool()>& kFunc) :
    func_(kFunc)
{
}

BT::Node::Status BT::Condition::TickNode(float delta_time)
{
    return func_() ? Node::Status::kSuccess : Node::Status::kFailure;
}
