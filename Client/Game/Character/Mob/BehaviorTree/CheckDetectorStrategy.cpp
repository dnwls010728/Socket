#include "pch.h"
#include "CheckDetectorStrategy.h"

#include "Actor/Actor.h"
#include "Character/Blackboard/Blackboard.h"
#include "Character/Blackboard/BlackboardKey.h"

BT::CheckDetectorStrategy::CheckDetectorStrategy(Blackboard::Blackboard* blackboard) :
    blackboard_(blackboard)
{
}

BT::Node::Status BT::CheckDetectorStrategy::TickNode(float delta_time)
{
    if (!blackboard_) return Node::Status::kFailure;
    Blackboard::BlackboardKey target_key = blackboard_->GetOrRegisterKey(L"Target");
    
    Actor* target = nullptr;
    if (!blackboard_->TryGetValue(target_key, target) || !target)
        return Node::Status::kFailure;

    return Node::Status::kSuccess;
}
