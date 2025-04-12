#pragma once

namespace Blackboard
{
    class Blackboard;
}

namespace BT
{
    class Node;
}

namespace BT
{
    class BehaviorTree
    {
    public:
        BehaviorTree();
        ~BehaviorTree() = default;

        void Init(const std::shared_ptr<Blackboard::Blackboard>& blackboard);

        std::vector<std::shared_ptr<Node>> GetNodes();

        FORCEINLINE void SetRoot(const std::shared_ptr<Node>& kNode) { root_ = kNode; }
        FORCEINLINE std::shared_ptr<Node> GetRoot() { return root_; }

        FORCEINLINE void SetBlackboard(const std::shared_ptr<Blackboard::Blackboard>& kBlackboard) { blackboard_ = kBlackboard; }
        FORCEINLINE std::shared_ptr<Blackboard::Blackboard> GetBlackboard() { return blackboard_; }

    private:
        void CollectNodes(std::vector<std::shared_ptr<Node>>& out_nodes, const std::shared_ptr<Node>& node);

        std::shared_ptr<Node> root_;
        std::shared_ptr<Blackboard::Blackboard> blackboard_;
    
    };
}
