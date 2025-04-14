#include "pch.h"
#include "Decorator.h"

BT::Decorator::Decorator(const std::wstring& kName) :
    Node(kName),
    child_(nullptr)
{
}

void BT::Decorator::Reset()
{
    if (child_) child_->Reset();
}

void BT::Decorator::AddChild(const std::shared_ptr<Node>& kNode)
{
    child_ = kNode;
}
