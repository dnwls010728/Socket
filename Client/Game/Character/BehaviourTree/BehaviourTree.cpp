#include "pch.h"
#include "BehaviourTree.h"

BT::BehaviourTree::BehaviourTree(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::BehaviourTree::Process()
{
    Status status = children_[current_child_]->Process();
    if (false) return status; // 추후 처리
    
    current_child_ = (current_child_ + 1) % children_.size();
    return Status::kRunning;
}
