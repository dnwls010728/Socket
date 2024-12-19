#include "pch.h"
#include "Bullet.h"

#include "Logger.h"
#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Character/Actor/CharacterBase.h"
#include "Physics/Physics2D.h"
#include "Windows/DX/Sprite.h"

Bullet::Bullet(const std::wstring& kName) :
    PooledObject(kName),
    life_timer_()
{
    SetLayer(ActorLayer::kBullet);
    
    circle_collider_ = AddComponent<CircleColliderComponent>(L"Collider");
    circle_collider_->SetRadius(.125f);
    circle_collider_->SetTrigger(true);
    
    rigid_body_ = AddComponent<RigidBody2DComponent>(L"RigidBody");
    rigid_body_->SetCollisionDetectionMode(CollisionDetectionMode::kContinuous);
    rigid_body_->SetFreezeRotation(true);
    rigid_body_->SetGravityScale(0.f);

    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");

    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Bullet\\02.png");
    renderer_->SetSprite(sprite_, L"02_0");
}

void Bullet::Deactivate()
{
    PooledObject::Deactivate();

    TimerManager* timer_manager = TimerManager::Get();
    if (timer_manager->GetTimerRemaining(life_timer_) > 0.f)
        timer_manager->ClearTimer(life_timer_);
}

void Bullet::PhysicsTick(float delta_time)
{
    PooledObject::PhysicsTick(delta_time);

    std::vector<Actor*> out_actors;
    bool is_hit = Physics2D::OverlapCircleAll(GetTransform()->GetPosition(), .125f, out_actors, static_cast<Type::uint16>(ActorLayer::kMob));
    if (is_hit)
    {
        CharacterBase* character = static_cast<CharacterBase*>(out_actors[0]);
        if (character)
        {
            character->OnDamaged(10.f);
            Deactivate();
        }
    }
}

void Bullet::OnEnable()
{
    PooledObject::OnEnable();

    life_timer_ = TimerManager::Get()->SetTimer(this, &Bullet::Deactivate, 1.f);

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
