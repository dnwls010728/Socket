#include "pch.h"
#include "RigidBody2DComponent.h"

#include "TransformComponent.h"
#include "Actor/Actor.h"
#include "box2d/box2d.h"
#include "Math/Math.h"
#include "Math/Vector2.h"

RigidBody2DComponent::RigidBody2DComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    body_type_(BodyType::kDynamic),
    sleep_mode_(SleepMode::kStartAwake),
    collision_detection_mode_(CollisionDetectionMode::kDiscrete),
    gravity_scale_(1.f),
    mass_(1.f),
    is_fixed_rotation_(false),
    use_auto_mass_(false)
{
}

void RigidBody2DComponent::SetBodyType(BodyType type)
{
    body_type_ = type;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetBodyTypeInternal(body_id);
        SetAutoMassInternal(body_id);
    }
}

void RigidBody2DComponent::SetGravityScale(float scale)
{
    gravity_scale_ = scale;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetGravityScaleInternal(body_id);
    }
}

void RigidBody2DComponent::SetSleepMode(SleepMode mode)
{
    sleep_mode_ = mode;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetSleepModeInternal(body_id);
    }
}

void RigidBody2DComponent::SetCollisionDetectionMode(CollisionDetectionMode mode)
{
    collision_detection_mode_ = mode;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetCollisionDetectionModeInternal(body_id);
    }
}

void RigidBody2DComponent::SetFreezeRotation(bool freeze)
{
    is_fixed_rotation_ = freeze;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetFixedRotationInternal(body_id);
        SetAutoMassInternal(body_id); // 질량을 다시 계산합니다.
    }
}

void RigidBody2DComponent::UseAutoMass(bool use_auto_mass)
{
    use_auto_mass_ = use_auto_mass;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetAutoMassInternal(body_id);
    }
}

void RigidBody2DComponent::SetMass(float mass)
{
    mass_ = mass;

    if (HasBegunPlay())
    {
        b2BodyId body_id = GetValidBodyId();
        SetMassInternal(body_id);
    }
}

void RigidBody2DComponent::SetPosition(const Math::Vector2& kPosition)
{
    b2BodyId body_id = GetValidBodyId();

    b2Body_SetTransform(body_id, {kPosition.x, kPosition.y}, b2Body_GetRotation(body_id));
}

void RigidBody2DComponent::SetAngle(float angle)
{
    b2BodyId body_id = GetValidBodyId();

    b2Body_SetTransform(body_id, b2Body_GetPosition(body_id), b2MakeRot(angle * MATH_PI / 180.f));
}

void RigidBody2DComponent::SetLinearVelocity(const Math::Vector2& kLinearVelocity)
{
    b2BodyId body_id = GetValidBodyId();
    
    b2Body_SetLinearVelocity(body_id, {kLinearVelocity.x, kLinearVelocity.y});
}

void RigidBody2DComponent::SetLinearVelocityX(float linear_velocity)
{
    b2BodyId body_id = GetValidBodyId();
    
    b2Body_SetLinearVelocity(body_id, {linear_velocity, b2Body_GetLinearVelocity(body_id).y});
}

void RigidBody2DComponent::SetLinearVelocityY(float linear_velocity)
{
    b2BodyId body_id = GetValidBodyId();
    
    b2Body_SetLinearVelocity(body_id, {b2Body_GetLinearVelocity(body_id).x, linear_velocity});
}

void RigidBody2DComponent::SetAngularVelocity(float velocity)
{
    b2BodyId body_id = GetValidBodyId();

    b2Body_SetAngularVelocity(body_id, velocity);
}

void RigidBody2DComponent::AddForce(const Math::Vector2& kForce, ForceMode mode)
{
    b2BodyId body_id = GetValidBodyId();

    switch (mode)
    {
    case ForceMode::kForce:
        b2Body_ApplyForceToCenter(body_id, {kForce.x, kForce.y}, true);
        break;

    case ForceMode::kImpulse:
        b2Body_ApplyLinearImpulseToCenter(body_id, {kForce.x, kForce.y}, true);
        break;
    }
}

void RigidBody2DComponent::AddForceX(float force, ForceMode mode)
{
    b2BodyId body_id = GetValidBodyId();

    switch (mode)
    {
    case ForceMode::kForce:
        b2Body_ApplyForceToCenter(body_id, {force, 0.f}, true);
        break;

    case ForceMode::kImpulse:
        b2Body_ApplyLinearImpulseToCenter(body_id, {force, 0.f}, true);
        break;
    }
}

void RigidBody2DComponent::AddForceY(float force, ForceMode mode)
{
    b2BodyId body_id = GetValidBodyId();

    switch (mode)
    {
    case ForceMode::kForce:
        b2Body_ApplyForceToCenter(body_id, {0.f, force}, true);
        break;

    case ForceMode::kImpulse:
        b2Body_ApplyLinearImpulseToCenter(body_id, {0.f, force}, true);
        break;
    }
}

void RigidBody2DComponent::AddForceAtPosition(const Math::Vector2& kForce, const Math::Vector2& kPosition, ForceMode mode)
{
    b2BodyId body_id = GetValidBodyId();

    switch (mode)
    {
    case ForceMode::kForce:
        b2Body_ApplyForce(body_id, {kForce.x, kForce.y}, {kPosition.x, kPosition.y}, true);
        break;

    case ForceMode::kImpulse:
        b2Body_ApplyLinearImpulse(body_id, {kForce.x, kForce.y}, {kPosition.x, kPosition.y}, true);
        break;
    }
}

void RigidBody2DComponent::AddTorque(float torque, ForceMode mode)
{
    b2BodyId body_id = GetValidBodyId();

    switch (mode)
    {
    case ForceMode::kForce:
        b2Body_ApplyTorque(body_id, torque, true);
        break;

    case ForceMode::kImpulse:
        b2Body_ApplyAngularImpulse(body_id, torque, true);
        break;
    }
}

void RigidBody2DComponent::Sleep()
{
    b2BodyId body_id = GetValidBodyId();

    b2Body_SetAwake(body_id, false);
}

void RigidBody2DComponent::WakeUp()
{
    b2BodyId body_id = GetValidBodyId();

    b2Body_SetAwake(body_id, true);
}

Math::Vector2 RigidBody2DComponent::GetPosition() const
{
    b2BodyId body_id = GetValidBodyId();

    b2Vec2 position = b2Body_GetPosition(body_id);
    return {position.x, position.y};
}

Math::Vector2 RigidBody2DComponent::GetLinearVelocity() const
{
    b2BodyId body_id = GetValidBodyId();

    b2Vec2 velocity = b2Body_GetLinearVelocity(body_id);
    return {velocity.x, velocity.y};
}

float RigidBody2DComponent::GetAngle() const
{
    b2BodyId body_id = GetValidBodyId();

    return b2Rot_GetAngle(b2Body_GetRotation(body_id)) * 180.f / MATH_PI;
}

float RigidBody2DComponent::GetAngularVelocity() const
{
    b2BodyId body_id = GetValidBodyId();

    return b2Body_GetAngularVelocity(body_id);
}

float RigidBody2DComponent::GetLinearVelocityX() const
{
    b2BodyId body_id = GetValidBodyId();

    return b2Body_GetLinearVelocity(body_id).x;
}

float RigidBody2DComponent::GetLinearVelocityY() const
{
    b2BodyId body_id = GetValidBodyId();

    return b2Body_GetLinearVelocity(body_id).y;
}

bool RigidBody2DComponent::IsAwake() const
{
    b2BodyId body_id = GetValidBodyId();

    return b2Body_IsAwake(body_id);
}

BodyType RigidBody2DComponent::GetBodyType() const
{
    b2BodyId body_id = GetValidBodyId();

    if (b2Body_GetType(body_id) == b2_kinematicBody) return BodyType::kKinematic;
    if (b2Body_GetType(body_id) == b2_staticBody) return BodyType::kStatic;
    return BodyType::kDynamic;
}

void RigidBody2DComponent::InitializeComponent()
{
    ActorComponent::InitializeComponent();
    
    if (!b2Body_IsValid(GetOwner()->body_id_)) GetOwner()->CreateBody();

    b2BodyId body_id = GetValidBodyId();
    
    SetBodyTypeInternal(body_id); // 강체의 유형을 설정합니다.
    SetGravityScaleInternal(body_id); // 중력의 크기를 설정합니다.
    SetSleepModeInternal(body_id); // 강체의 수면 모드를 설정합니다.
    SetCollisionDetectionModeInternal(body_id); // 충돌 감지 모드를 설정합니다.
    SetFixedRotationInternal(body_id); // 회전을 고정할지 여부를 설정합니다.
    SetAutoMassInternal(body_id); // 자동으로 질량을 계산할지 여부를 설정합니다.
}

void RigidBody2DComponent::SetBodyTypeInternal(b2BodyId body_id)
{
    switch (body_type_)
    {
    case BodyType::kDynamic:
        b2Body_SetType(body_id, b2_dynamicBody);
        break;

    case BodyType::kKinematic:
        b2Body_SetType(body_id, b2_kinematicBody);
        break;

    case BodyType::kStatic:
        b2Body_SetType(body_id, b2_staticBody);
        break;
    }
}

void RigidBody2DComponent::SetGravityScaleInternal(b2BodyId body_id)
{
    b2Body_SetGravityScale(body_id, gravity_scale_);
}

void RigidBody2DComponent::SetSleepModeInternal(b2BodyId body_id)
{
    switch (sleep_mode_)
    {
    case SleepMode::kNeverSleep:
        b2Body_EnableSleep(body_id, false);
        break;

    case SleepMode::kStartAwake:
        b2Body_EnableSleep(body_id, true);
        break;
    }
}

void RigidBody2DComponent::SetCollisionDetectionModeInternal(b2BodyId body_id)
{
    switch (collision_detection_mode_)
    {
    case CollisionDetectionMode::kDiscrete:
        b2Body_SetBullet(body_id, false);
        break;

    case CollisionDetectionMode::kContinuous:
        b2Body_SetBullet(body_id, true);
        break;
    }
}

void RigidBody2DComponent::SetFixedRotationInternal(b2BodyId body_id)
{
    b2Body_SetFixedRotation(body_id, is_fixed_rotation_);
}

void RigidBody2DComponent::SetAutoMassInternal(b2BodyId body_id)
{
    if (use_auto_mass_)
    {
        b2Body_ApplyMassFromShapes(body_id);
        return;
    }

    SetMassInternal(body_id);
}

void RigidBody2DComponent::SetMassInternal(b2BodyId body_id)
{
    b2MassData mass_data = b2Body_GetMassData(body_id);
    mass_data.mass = mass_;

    b2Body_SetMassData(body_id, mass_data);
}

b2BodyId RigidBody2DComponent::GetValidBodyId() const
{
    Actor* owner = GetOwner();
    CHECK(owner);

    b2BodyId body_id = owner->body_id_;
    CHECK(b2Body_IsValid(body_id));

    return body_id;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<RigidBody2DComponent>("RigidBody2DComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
