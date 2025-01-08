#include "pch.h"
#include "ActionStrategy.h"

BT::ActionStrategy::ActionStrategy(const Function<void()>& kFunc) :
    func_(kFunc)
{
}

BT::Node::Status BT::ActionStrategy::TickNode(float delta_time)
{
    func_();
    return Node::Status::kSuccess;
}
