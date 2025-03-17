#include "pch.h"
#include "CircleColliderComponent.h"

#include "Actor/Actor.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"

CircleColliderComponent::CircleColliderComponent(Actor* owner, const std::wstring& kName) :
    ColliderComponent(owner, kName),
    radius_(.5f)
{
}

void CircleColliderComponent::SetRadius(float radius)
{
    radius_ = radius;

    if (HasBegunPlay())
    {
        SetShape();
    }
}

void CircleColliderComponent::SetShape()
{
    b2Circle circle = {
        {offset_.x, offset_.y},
        radius_
    };
    
    if (b2Shape_IsValid(shape_id_))
    {
        b2Shape_SetCircle(shape_id_, &circle);
    }
    else
    {
        b2Filter filter = b2DefaultFilter();
        filter.categoryBits = static_cast<Type::uint16>(GetOwner()->GetLayer());
        filter.maskBits = static_cast<Type::uint16>(EngineSettings::Get()->GetCollisionLayer(GetOwner()->GetLayer()));
        
        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.density = 1.f;
        shape_def.friction = material_.friction;
        shape_def.restitution = material_.bounciness;
        shape_def.filter = filter;
        shape_def.userData = this;
        shape_def.isSensor = is_trigger_;

        shape_id_ = b2CreateCircleShape(GetOwner()->body_id_, &shape_def, &circle);
    }
}

void CircleColliderComponent::SetTriggerInternal()
{
    CHECK(b2Shape_IsValid(shape_id_));

    b2Circle circle = b2Shape_GetCircle(shape_id_);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = b2Shape_GetDensity(shape_id_);
    shape_def.friction = b2Shape_GetFriction(shape_id_);
    shape_def.restitution = b2Shape_GetRestitution(shape_id_);
    shape_def.filter = b2Shape_GetFilter(shape_id_);
    shape_def.isSensor = is_trigger_;

    b2DestroyShape(shape_id_, true);
    shape_id_ = b2CreateCircleShape(GetOwner()->body_id_, &shape_def, &circle);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CircleColliderComponent>("CircleColliderComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
