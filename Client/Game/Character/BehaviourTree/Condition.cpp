#include "pch.h"
#include "Condition.h"

BT::Condition::Condition(const Function<bool()>& kFunc) :
    func_(kFunc)
{
}

BT::Node::Status BT::Condition::Process()
{
    return func_() ? Node::Status::kSuccess : Node::Status::kFailure;
}
