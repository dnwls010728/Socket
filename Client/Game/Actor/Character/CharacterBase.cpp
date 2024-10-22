#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/Controller2DComponent.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName),
    gravity_(-9.8f),
    velocity_(Math::Vector2::Zero())
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");
    collider_ = AddComponent<CapsuleColliderComponent>(L"Collider");
    controller_ = AddComponent<Controller2DComponent>(L"Controller");
    
}

bool CharacterBase::IsGrounded() const
{
    return controller_->GetCollisions().below;
}

void CharacterBase::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    velocity_.y += gravity_ * delta_time;

    controller_->Move(velocity_ * delta_time);

    const CollisionInfo& collisions = controller_->GetCollisions();
    if (collisions.below || collisions.above)
    {
        if (collisions.sliding_down_max_slope)
            velocity_.y += collisions.slope_normal.y * -gravity_ * delta_time;
        else velocity_.y = 0.f;
    }
    
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<CharacterBase>("CharacterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
