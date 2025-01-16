#include "pch.h"
#include "MoveToLocationStrategy.h"

#include "Actor/Actor.h"
#include "Actor/Component/ColliderComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Character/Blackboard/Blackboard.h"
#include "Character/ContextSteering/ContextSteeringComponent.h"

BT::MoveToLocationStrategy::MoveToLocationStrategy(Blackboard::Blackboard* blackboard) :
    blackboard_(blackboard),
    is_init_(false),
    self_(nullptr),
    distance_threshold_(.2f),
    previous_stopping_distance_(0.f),
    context_steering_(nullptr),
    animator_(nullptr)
{
}

BT::Node::Status BT::MoveToLocationStrategy::TickNode(float delta_time)
{
    if (!blackboard_) return Node::Status::kFailure;
    
    if (!is_init_)
    {
        is_init_ = true;
        
        self_key_ = blackboard_->GetOrRegisterKey(L"Self");
        location_key_ = blackboard_->GetOrRegisterKey(L"Location");

        if (Validate() != Node::Status::kRunning)
        {
            Reset();
            return Node::Status::kFailure;
        }

        Math::Vector2 agent_position = Math::Vector2::Zero();
        Math::Vector2 location_position = Math::Vector2::Zero();
        if (GetDistanceToLcoation(agent_position, location_position) <= distance_threshold_)
        {
            Reset();
            return Node::Status::kFailure;
        }

        ActorComponent* animator_component = self_->GetComponent(AnimatorComponent::StaticClass());
        if (animator_component)
        {
            animator_ = static_cast<AnimatorComponent*>(animator_component);
            if (animator_) animator_->SetFloat(L"Speed", 1.f);
        }
        
        ActorComponent* context_steering_component = self_->GetComponent(ContextSteeringComponent::StaticClass());
        if (context_steering_component)
        {
            context_steering_ = static_cast<ContextSteeringComponent*>(context_steering_component);
            if (context_steering_)
            {
                if (!context_steering_->IsStopped()) context_steering_->Stop();
                
                context_steering_->SetSpeed(1.f);
                previous_stopping_distance_ = context_steering_->GetStoppingDistance();
                
                context_steering_->SetStoppingDistance(distance_threshold_);
                context_steering_->SetDestination(location_position);
            }
        }
    }
    
    if (Validate() != Node::Status::kRunning)
    {
        Reset();
        return Node::Status::kFailure;
    }

    if (context_steering_)
    {
        if (context_steering_->IsComplete())
        {
            Reset();
            return Node::Status::kSuccess;
        }
    }
    
    return Node::Status::kRunning;
}

void BT::MoveToLocationStrategy::Reset()
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

BT::Node::Status BT::MoveToLocationStrategy::Validate()
{
    if (!blackboard_->TryGetValue(self_key_, self_) || !self_) return Node::Status::kFailure;

    Math::Vector2 location_position = Math::Vector2::Zero();
    if (!blackboard_->TryGetValue(location_key_, location_position)) return Node::Status::kFailure;
    
    return Node::Status::kRunning;
}

float BT::MoveToLocationStrategy::GetDistanceToLcoation(Math::Vector2& agent_position, Math::Vector2& location_position)
{
    agent_position = self_->GetTransform()->GetPosition();
    blackboard_->TryGetValue(location_key_, location_position);
    return Math::Vector2::Distance(agent_position, location_position);
}
