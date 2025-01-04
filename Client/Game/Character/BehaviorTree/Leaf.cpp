#include "pch.h"
#include "Leaf.h"

#include "IStrategy.h"

BT::Leaf::Leaf(const std::wstring& kName, const std::shared_ptr<IStrategy>& strategy) :
    Node(kName),
    strategy_(strategy)
{
}

BT::Node::Status BT::Leaf::TickNode(float delta_time)
{
    return strategy_->TickNode(delta_time);
}

void BT::Leaf::Reset()
{
    strategy_->Reset();
}
