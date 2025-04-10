#include "pch.h"
#include "Composite.h"

BT::Composite::Composite(const std::wstring& kName) :
    Node(kName),
    children_(),
    current_child_(0)
{
}

void BT::Composite::Reset()
{
    current_child_ = 0;
    for (auto& child : children_)
    {
        child->Reset();
    }
}

void BT::Composite::AddChild(const std::shared_ptr<Node>& kNode)
{
    children_.push_back(kNode);
}
