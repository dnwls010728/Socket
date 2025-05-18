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
    const auto& collisions = controller_->GetCollisions();
    if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;

    velocity_.x = 0.5f;
    velocity_.y += gravity_ * delta_time;
    controller_->Move(velocity_ * delta_time);
    
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