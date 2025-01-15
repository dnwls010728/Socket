#include "pch.h"
#include "MoveToTargetStrategy.h"

#include "Actor/Actor.h"
#include "Actor/Component/ColliderComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Blackboard/Blackboard.h"
#include "Character/ContextSteering/ContextSteering.h"

BT::MoveToTargetStrategy::MoveToTargetStrategy(Blackboard::Blackboard* blackboard) :
    blackboard_(blackboard),
    is_init_(false),
    self_(nullptr),
    target_(nullptr),
    last_position_(Math::Vector2::Zero()),
    collider_adjusted_position_(Math::Vector2::Zero()),
    distance_threshold_(.2f),
    collider_offset_(0.f),
    previous_stopping_distance_(0.f),
    context_steering_(nullptr),
    animator_(nullptr)
{
}

BT::Node::Status BT::MoveToTargetStrategy::TickNode(float delta_time)
{
    if (!blackboard_) return Node::Status::kFailure;
    
    if (!is_init_)
    {
        is_init_ = true;
        
        self_key_ = blackboard_->GetOrRegisterKey(L"Self");
        target_key_ = blackboard_->GetOrRegisterKey(L"Target");

        if (Validate() != Node::Status::kRunning)
        {
            Reset();
            return Node::Status::kFailure;
        }

        last_position_ = self_->GetTransform()->GetPosition();
        collider_adjusted_position_ = GetPositionColliderAdjusted();

        collider_offset_ = 0.f;
        if (GetDistance() <= (distance_threshold_ + collider_offset_))
        {
            Reset();
            return Node::Status::kSuccess;
        }

        ActorComponent* animator_component = self_->GetComponent(AnimatorComponent::StaticClass());
        if (animator_component)
        {
            animator_ = static_cast<AnimatorComponent*>(animator_component);
            if (animator_) animator_->SetFloat(L"Speed", 1.f);
        }

        ActorComponent* context_steering_component = self_->GetComponent(ContextSteering::StaticClass());
        if (context_steering_component)
        {
            context_steering_ = static_cast<ContextSteering*>(context_steering_component);
            if (context_steering_)
            {
                if (!context_steering_->IsStopped()) context_steering_->Stop();
                
                context_steering_->SetSpeed(1.f);
                previous_stopping_distance_ = context_steering_->GetStoppingDistance();
                
                context_steering_->SetStoppingDistance(distance_threshold_ + collider_offset_);
                context_steering_->SetDestination(collider_adjusted_position_);
            }
        }
    }
    
    if (Validate() != Node::Status::kRunning)
    {
        Reset();
        return Node::Status::kFailure;
    }

    bool is_update_position = last_position_ != target_->GetTransform()->GetPosition();
    if (is_update_position)
    {
        last_position_ = target_->GetTransform()->GetPosition();
        collider_adjusted_position_ = GetPositionColliderAdjusted();
    }

    float distance = GetDistance();
    if (distance <= (distance_threshold_ + collider_offset_))
    {
        Reset();
        return Node::Status::kSuccess;
    }

    if (context_steering_)
    {
        if (is_update_position) context_steering_->SetDestination(collider_adjusted_position_);
        if (context_steering_->IsComplete())
        {
            Reset();
            return Node::Status::kSuccess;
        }
    }
    
    return Node::Status::kRunning;
}

void BT::MoveToTargetStrategy::Reset()
{
    if (animator_) animator_->SetFloat(L"Speed", 0.f);
    
    if (context_steering_)
    {
        context_steering_->SetStoppingDistance(previous_stopping_distance_);
        if (!context_steering_->IsStopped()) context_steering_->Stop();
    }
    context_steering_ = nullptr;
    
    is_init_ = false;
}

BT::Node::Status BT::MoveToTargetStrategy::Validate()
{
    if (!blackboard_->TryGetValue(self_key_, self_) || !self_) return Node::Status::kFailure;
    if (!blackboard_->TryGetValue(target_key_, target_) || !target_) return Node::Status::kFailure;
    return Node::Status::kRunning;
}

Math::Vector2 BT::MoveToTargetStrategy::GetPositionColliderAdjusted() const
{
    ActorComponent* component = target_->GetComponent(ColliderComponent::StaticClass());
    if (!component)
    {
        ColliderComponent* collider = static_cast<ColliderComponent*>(component);
        if (collider) return collider->ClosestPoint(self_->GetTransform()->GetPosition());
    }

    return target_->GetTransform()->GetPosition();
}

float BT::MoveToTargetStrategy::GetDistance()
{
    return Math::Vector2::Distance(self_->GetTransform()->GetPosition(), collider_adjusted_position_);
}
