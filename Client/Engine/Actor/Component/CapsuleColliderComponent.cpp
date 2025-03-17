#include "pch.h"
#include "CapsuleColliderComponent.h"

#include "Actor/Actor.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"

CapsuleColliderComponent::CapsuleColliderComponent(Actor* owner, const std::wstring& kName) :
    ColliderComponent(owner, kName),
    direction_(CapsuleDirection::Vertical),
    size_(Math::Vector2::One())
{
}

void CapsuleColliderComponent::SetSize(Math::Vector2 size)
{
    size_ = size;

    if (HasBegunPlay())
    {
        SetShape();
    }
}

void CapsuleColliderComponent::SetDirection(CapsuleDirection direction)
{
    direction_ = direction;

    if (HasBegunPlay())
    {
        SetShape();
    }
}

void CapsuleColliderComponent::SetShape()
{
    const Math::Vector2 half_size = size_ * .5f;
    
    b2Capsule capsule;

    if (direction_ == CapsuleDirection::Horizontal)
    {
        capsule.center1 = {offset_.x - half_size.x, offset_.y};
        capsule.center2 = {offset_.x + half_size.x, offset_.y};
        capsule.radius = half_size.y;
    }
    else
    {
        capsule.center1 = {offset_.x, offset_.y - half_size.y};
        capsule.center2 = {offset_.x, offset_.y + half_size.y};
        capsule.radius = half_size.x;
    }

    if (b2Shape_IsValid(shape_id_))
    {
        b2Shape_SetCapsule(shape_id_, &capsule);
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
    
        shape_id_ = b2CreateCapsuleShape(GetOwner()->body_id_, &shape_def, &capsule);
    }
}

void CapsuleColliderComponent::SetTriggerInternal()
{
    if (!b2Shape_IsValid(shape_id_)) return;

    b2Capsule capsule = b2Shape_GetCapsule(shape_id_);
    
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = b2Shape_GetDensity(shape_id_);
    shape_def.friction = b2Shape_GetFriction(shape_id_);
    shape_def.restitution = b2Shape_GetRestitution(shape_id_);
    shape_def.filter = b2Shape_GetFilter(shape_id_);
    shape_def.isSensor = is_trigger_;

    b2DestroyShape(shape_id_, true);
    shape_id_ = b2CreateCapsuleShape(GetOwner()->body_id_, &shape_def, &capsule);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CapsuleColliderComponent>("CapsuleColliderComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
