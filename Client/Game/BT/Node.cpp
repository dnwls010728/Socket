#include "pch.h"
#include "Node.h"

BT::Node::Node(const std::wstring& kName) :
    name_(kName),
    children_(),
    current_child_(0)
{
}

BT::Node::Status BT::Node::TickNode(float delta_time)
{
    return Status::kSuccess;
}

void BT::Node::Reset()
{
    current_child_ = 0;
    for (const auto& kChild : children_)
    {
        kChild->Reset();
    }
}

void BT::Node::AddChild(const std::shared_ptr<Node>& kNode)
{
    children_.push_back(kNode);
}
