#include "pch.h"
#include "Bullet.h"

#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

Bullet::Bullet(const std::wstring& kName) :
    PooledObject(kName)
{
    SetLayer(ActorLayer::kBullet);
    
    circle_collider_ = AddComponent<CircleColliderComponent>(L"Collider");
    circle_collider_->SetRadius(.125f);
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody");
    rigid_body_->SetCollisionDetectionMode(CollisionDetectionMode::kContinuous);
    rigid_body_->SetFreezeRotation(true);
    rigid_body_->SetGravityScale(0.f);

    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");

    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Bullet\\02.png");
    renderer_->SetSprite(sprite_, L"02_0");
}

void Bullet::OnEnable()
{
    PooledObject::OnEnable();

    Math::Vector2 direction = GetTransform()->GetRightVector();

    rigid_body_->AddForce(direction * 20.f, ForceMode::kImpulse);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Bullet>("Bullet")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
