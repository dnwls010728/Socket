#include "pch.h"
#include "Condition.h"

BT::Condition::Condition(const Function<bool()>& func) :
    func_(func)
{
}

BT::Node::Status BT::Condition::Process()
{
    return func_() ? Node::Status::kSuccess : Node::Status::kFailure;
}
