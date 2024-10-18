#include "pch.h"
#include "Bullet.h"

#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Resource/ResourceManager.h"
#include "Windows/DX/Sprite.h"

Bullet::Bullet(const std::wstring& kName) :
    PooledObject(kName)
{
    SetLayer(ActorLayer::kBullet);
    
    circle_collider_ = AddComponent<CircleColliderComponent>(L"Collider");
    circle_collider_->SetRadius(.125f);
    // circle_collider_->SetTrigger(true);
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody");
    rigid_body_->SetCollisionDetectionMode(CollisionDetectionMode::kContinuous);

    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");
    
    if (ResourceManager::Get()->Load<Sprite>(L"Circle", L".\\Game_Data\\Default\\Circle.png"))
    {
        sprite_ = ResourceManager::Get()->GetResource<Sprite>(L"Circle");
        sprite_->SetPPU(256);
    }

    sprite_->Split(1, 1, Sprite::kCenter);
    sprite_->SetFilterMode(FilterMode::kBilinear);

    renderer_->SetSprite(sprite_);

    GetTransform()->SetScale({.25f, .25f});
}

void Bullet::OnEnable()
{
    PooledObject::OnEnable();

    rigid_body_->AddForceX(20.f, ForceMode::kImpulse);
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
