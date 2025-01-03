#include "pch.h"
#include "Node.h"

BT::Node::Node(const std::wstring& kName) :
    name_(kName),
    children_(),
    current_child_(0)
{
}

void BT::Node::AddChild(Node* child)
{
    children_.push_back(child);
}

void BT::Node::Reset()
{
    current_child_ = 0;
    for (const auto& child : children_)
    {
        child->Reset();
    }
}

BT::Node::Status BT::Node::Process()
{
    return Status::kSuccess;
}
