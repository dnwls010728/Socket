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
    if (!strategy_) return Status::kFailure;
    return strategy_->TickNode(delta_time);
}

void BT::Leaf::Reset()
{
    if (strategy_) strategy_->Reset();
}

std::shared_ptr<BT::Leaf> BT::Leaf::Create(const std::wstring& kName, const std::shared_ptr<IStrategy>& strategy)
{
    return std::make_shared<Leaf>(kName, strategy);
}
