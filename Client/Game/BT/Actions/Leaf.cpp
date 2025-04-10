#include "pch.h"
#include "Leaf.h"

#include "IStrategy.h"

BT::Leaf::Leaf(const std::wstring& kName) 
    : Node(kName)
{
}

BT::Node::Status BT::Leaf::TickNode(float delta_time)
{
    if (!strategy_) return Status::kFailure;
    return strategy_->TickNode(delta_time);
}

void BT::Leaf::Reset()
{
    if (strategy_) strategy_->Reset();
}
