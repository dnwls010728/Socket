#include "pch.h"
#include "Node.h"

BT::Node::Node(const std::wstring& kName) :
    name_(kName),
    behavior_tree_(nullptr)
{
}

BT::Node::Status BT::Node::TickNode(float delta_time)
{
    return Status::kSuccess;
}

void BT::Node::Reset()
{
}
