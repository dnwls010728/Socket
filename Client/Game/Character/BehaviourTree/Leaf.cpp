#include "pch.h"
#include "Leaf.h"

#include "IStrategy.h"

BT::Leaf::Leaf(const std::wstring& kName, const std::shared_ptr<IStrategy>& strategy) :
    Node(kName),
    strategy_(strategy)
{
}

void BT::Leaf::Reset()
{
    strategy_->Reset();
}

BT::Node::Status BT::Leaf::Process()
{
    return strategy_->Process();
}
