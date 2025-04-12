#include "pch.h"
#include "BehaviorTree.h"

#include "Composites/Composite.h"
#include "Decorators/Decorator.h"

BT::BehaviorTree::BehaviorTree() :
    root_(nullptr),
    blackboard_(nullptr)
{
}

void BT::BehaviorTree::Init(const std::shared_ptr<Blackboard::Blackboard>& blackboard)
{
    blackboard_ = blackboard;
    for (const auto& node : GetNodes())
    {
        node->behavior_tree_ = this;
    }
}

std::vector<std::shared_ptr<BT::Node>> BT::BehaviorTree::GetNodes()
{
    std::vector<std::shared_ptr<Node>> nodes;
    CollectNodes(nodes, root_);
    return nodes;
}

void BT::BehaviorTree::CollectNodes(std::vector<std::shared_ptr<Node>>& out_nodes, const std::shared_ptr<Node>& node)
{
    out_nodes.push_back(node);
    if (const std::shared_ptr<Decorator> decorator = std::dynamic_pointer_cast<Decorator>(node))
    {
        CollectNodes(out_nodes, decorator->GetChild());
    }
    else if (const std::shared_ptr<Composite> composite = std::dynamic_pointer_cast<Composite>(node))
    {
        for (const auto& child : composite->GetChildren())
        {
            CollectNodes(out_nodes, child);
        }
    }
}
