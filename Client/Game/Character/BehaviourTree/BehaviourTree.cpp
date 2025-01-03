#include "pch.h"
#include "BehaviourTree.h"

BT::BehaviourTree::BehaviourTree(const std::wstring& kName) :
    Node(kName)
{
}

BT::Node::Status BT::BehaviourTree::Process()
{
    while (current_child_ < children_.size())
    {
        Status status = children_[current_child_]->Process();
        if (status != Status::kSuccess) return status;
        current_child_++;
    }

    return Status::kSuccess;
}
