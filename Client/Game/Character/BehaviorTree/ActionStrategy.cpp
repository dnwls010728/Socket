#include "pch.h"
#include "ActionStrategy.h"

BT::ActionStrategy::ActionStrategy(const Function<void()>& kFunc) :
    func_(kFunc)
{
}

BT::Node::Status BT::ActionStrategy::Process()
{
    func_();
    return Node::Status::kSuccess;
}
