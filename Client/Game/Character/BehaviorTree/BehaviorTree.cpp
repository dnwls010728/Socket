#include "pch.h"
#include "BehaviorTree.h"

BT::BehaviorTree::BehaviorTree(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::BehaviorTree::TickNode(float delta_time)
{
    Status status = children_[current_child_]->TickNode(delta_time);
    if (false) return status; // 추후 처리
    
    current_child_ = (current_child_ + 1) % children_.size();
    return Status::kRunning;
}
