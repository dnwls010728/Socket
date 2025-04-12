#include "pch.h"
#include "BehaviorTreeComponent.h"

#include "BT/BehaviorTree.h"

BehaviorTreeComponent::BehaviorTreeComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName)
{
    behavior_tree_ = std::make_shared<BT::BehaviorTree>();
}

std::shared_ptr<Blackboard::Blackboard> BehaviorTreeComponent::GetBlackboard()
{
    if (behavior_tree_) return behavior_tree_->GetBlackboard();
    return nullptr;
}

void BehaviorTreeComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();

    if (behavior_tree_) behavior_tree_->Init();
}

void BehaviorTreeComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    if (behavior_tree_) behavior_tree_->Tick(delta_time);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<BehaviorTreeComponent>("BehaviorTreeComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
