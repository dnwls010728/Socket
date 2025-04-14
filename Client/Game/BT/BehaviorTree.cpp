#include "pch.h"
#include "BehaviorTree.h"

#include "Composites/Composite.h"
#include "Decorators/Decorator.h"
#include "Decorators/Start.h"

BT::BehaviorTree::BehaviorTree() :
    root_(nullptr)
{
    blackboard_ = std::make_shared<Blackboard::Blackboard>();
}

void BT::BehaviorTree::Init()
{
    for (const auto& node : GetNodes())
    {
        node->behavior_tree_ = shared_from_this();
        Logger::Print(L"Node: %s", node->GetName().c_str());
    }
}

void BT::BehaviorTree::Tick(float delta_time)
{
    if (root_)
    {
        Node::Status status = root_->TickNode(delta_time);
    }
}

std::vector<std::shared_ptr<BT::Node>> BT::BehaviorTree::GetNodes()
{
    std::vector<std::shared_ptr<Node>> nodes;
    if (root_) CollectNodes(nodes, root_);
    return nodes;
}

void BT::BehaviorTree::CollectNodes(std::vector<std::shared_ptr<Node>>& out_nodes, const std::shared_ptr<Node>& node)
{
    out_nodes.push_back(node);
    if (const std::shared_ptr<Decorator> decorator = std::dynamic_pointer_cast<Decorator>(node))
    {
        if (decorator->GetChild()) CollectNodes(out_nodes, decorator->GetChild());
    }
    else if (const std::shared_ptr<Composite> composite = std::dynamic_pointer_cast<Composite>(node))
    {
        for (const auto& child : composite->GetChildren())
        {
            CollectNodes(out_nodes, child);
        }
    }
}
