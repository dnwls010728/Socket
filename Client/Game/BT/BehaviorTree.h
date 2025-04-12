#pragma once
#include "Blackboard/Blackboard.h"

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
    class BehaviorTree : public std::enable_shared_from_this<BehaviorTree>
    {
    public:
        BehaviorTree();
        ~BehaviorTree() = default;

        void Init();
        void Tick(float delta_time);

        std::vector<std::shared_ptr<Node>> GetNodes();

        template <typename T>
        bool TryGetValue(const std::wstring& name, T& out_value) const;
        
        template <typename T>
        void SetValue(const std::wstring& name, const T& value);

        FORCEINLINE void SetRoot(const std::shared_ptr<Node>& kNode) { root_ = kNode; }
        FORCEINLINE std::shared_ptr<Node> GetRoot() { return root_; }

        FORCEINLINE void SetBlackboard(const std::shared_ptr<Blackboard::Blackboard>& kBlackboard) { blackboard_ = kBlackboard; }
        FORCEINLINE std::shared_ptr<Blackboard::Blackboard> GetBlackboard() { return blackboard_; }

    private:
        void CollectNodes(std::vector<std::shared_ptr<Node>>& out_nodes, const std::shared_ptr<Node>& node);

        std::shared_ptr<Node> root_;
        std::shared_ptr<Blackboard::Blackboard> blackboard_;
    
    };
    
    template <typename T>
    bool BehaviorTree::TryGetValue(const std::wstring& name, T& out_value) const
    {
        if (!blackboard_)
        {
            out_value = T();
            return false;
        }
        
        Blackboard::BlackboardKey key = blackboard_->FindOrAdd(name);
        if (!key.IsValid())
        {
            out_value = T();
            return false;
        }
        
        return blackboard_->TryGetValue<T>(key, out_value);
    }

    template <typename T>
    void BehaviorTree::SetValue(const std::wstring& name, const T& value)
    {
        if (!blackboard_) return;
        Blackboard::BlackboardKey key = blackboard_->FindOrAdd(name);
        
        if (!key.IsValid()) return;
        blackboard_->SetValue<T>(key, value);
    }
}
