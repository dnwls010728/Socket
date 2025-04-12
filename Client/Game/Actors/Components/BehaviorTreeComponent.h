#pragma once
#include "Actor/Component/ActorComponent.h"
#include "BT/BehaviorTree.h"

namespace Blackboard
{
    class Blackboard;
}

namespace BT
{
    class BehaviorTree;
}

class BehaviorTreeComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(BehaviorTreeComponent)
    GENERATED_BODY(BehaviorTreeComponent, ActorComponent)
    
public:
    BehaviorTreeComponent(Actor* owner, const std::wstring& kName);
    virtual ~BehaviorTreeComponent() override = default;

    std::shared_ptr<Blackboard::Blackboard> GetBlackboard();
    
    template <typename T>
    bool TryGetValue(const std::wstring& name, T& out_value) const;

    template <typename T>
    void SetValue(const std::wstring& name, const T& value);

protected:
    virtual void InitializeComponent() override;
    virtual void TickComponent(float delta_time) override;

private:
    std::shared_ptr<BT::BehaviorTree> behavior_tree_;
    
};

template <typename T>
bool BehaviorTreeComponent::TryGetValue(const std::wstring& name, T& out_value) const
{
    if (behavior_tree_) return behavior_tree_->TryGetValue<T>(name, out_value);

    out_value = T();
    return false;
}

template <typename T>
void BehaviorTreeComponent::SetValue(const std::wstring& name, const T& value)
{
    if (behavior_tree_) behavior_tree_->SetValue<T>(name, value);
}
