#include "pch.h"
#include "PatrolStrategy.h"

#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/Animator/Animation.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Math/Math.h"

BT::PatrolStrategy::PatrolStrategy(AnimatorComponent* animator, RigidBody2DComponent* rigid_body) :
    animator_(animator),
    rigid_body_(rigid_body),
    timer_(0.f),
    direction_(1)
{
}

BT::Node::Status BT::PatrolStrategy::TickNode(float delta_time)
{
    timer_ += delta_time;
    if (timer_ > 1.f)
    {
        timer_ = 0.f;
        direction_ = Math::RandRange(0, 1) == 0 ? -1 : 1;
    }

    rigid_body_->SetLinearVelocityX(direction_);
    return Node::Status::kRunning;
}

void BT::PatrolStrategy::Reset()
{
    timer_ = 0.f;
}
