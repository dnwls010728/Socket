#include "pch.h"
#include "BoxColliderComponent.h"

#include "Actor/Actor.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"

BoxColliderComponent::BoxColliderComponent(Actor* owner, const std::wstring& kName) :
    ColliderComponent(owner, kName),
    size_(Math::Vector2::One())
{
}

void BoxColliderComponent::SetSize(Math::Vector2 size)
{
    size_ = size;

    if (HasBegunPlay())
    {
        SetShape();
    }
}

void BoxColliderComponent::SetShape()
{
    b2Polygon box = b2MakeOffsetBox(size_.x * .5f, size_.y * .5f, {offset_.x, offset_.y}, b2Rot_identity);

    if (b2Shape_IsValid(shape_id_))
    {
        b2Shape_SetPolygon(shape_id_, &box);
    }
    else
    {
        b2Filter filter = b2DefaultFilter();
        // TODO :
        //filter.categoryBits = static_cast<Type::uint16>(GetOwner()->GetLayer());
        //filter.maskBits = static_cast<Type::uint16>(EngineSettings::Get()->GetCollisionLayer(GetOwner()->GetLayer()));
        
        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.density = 1.f;
        shape_def.material.friction = material_.friction;
        shape_def.material.restitution = material_.bounciness;
        shape_def.filter = filter;
        shape_def.userData = this;
        shape_def.isSensor = is_trigger_;

        shape_id_ = b2CreatePolygonShape(GetOwner()->body_id_, &shape_def, &box);
    }
}

void BoxColliderComponent::SetTriggerInternal()
{
    CHECK(b2Shape_IsValid(shape_id_));

    b2Polygon box = b2Shape_GetPolygon(shape_id_);
    
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = b2Shape_GetDensity(shape_id_);
    shape_def.material.friction = b2Shape_GetFriction(shape_id_);
    shape_def.material.restitution = b2Shape_GetRestitution(shape_id_);
    shape_def.filter = b2Shape_GetFilter(shape_id_);
    shape_def.isSensor = is_trigger_;

    b2DestroyShape(shape_id_, true);
    shape_id_ = b2CreatePolygonShape(GetOwner()->body_id_, &shape_def, &box);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<BoxColliderComponent>("BoxColliderComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
