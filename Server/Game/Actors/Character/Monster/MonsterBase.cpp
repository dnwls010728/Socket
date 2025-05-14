#include "pch.h"
#include "MonsterBase.h"

#include "Actor/Component/TransformComponent.h"
#include "Actors/Character/Components/Controller2DComponent.h"

MonsterBase::MonsterBase(std::wstring kName) : CharacterBase(kName)
{
}

void MonsterBase::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);
}

void MonsterBase::PhysicsTick(float delta_time)
{
    const std::shared_ptr<TransformComponent> transform = GetTransform();
    const Controller2DComponent::CollisionInfo& collisions = controller_->GetCollisions();

    // 테스트
    std::cout<<"Before x : "<< transform->GetPosition().x << std::endl;
    std::cout<<"Before y : "<< transform->GetPosition().y << std::endl;
    
    Math::Vector2 movement_input_ = {1.0f,0.0f};
    velocity_.x = movement_input_.x * 1.f;
    velocity_.y = 0;
    
    controller_->Move(velocity_ * delta_time, movement_input_);

    if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;

    std::cout<<"After x : "<< transform->GetPosition().x << std::endl;
    std::cout<<"After y : "<< transform->GetPosition().y << std::endl;
    
    CharacterBase::PhysicsTick(delta_time);
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MonsterBase>("MonsterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}