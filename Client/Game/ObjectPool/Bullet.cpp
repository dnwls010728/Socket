#include "pch.h"
#include "Bullet.h"

#include "HitEffect.h"
#include "Logger.h"
#include "ObjectPoolComponent.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Character/CharacterBase.h"
#include "Math/Bounds.h"
#include "Math/Math.h"
#include "Misc/GameplayStatics.h"
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

    hit_effect_pool_ = AddComponent<ObjectPoolComponent>(L"HitEffectPool");
    hit_effect_pool_->SetPooledObjectClass(HitEffect::StaticClass());
}

void Bullet::Deactivate()
{
    PooledObject::Deactivate();

    TimerManager* timer_manager = TimerManager::Get();
    if (life_timer_.IsValid() && timer_manager->GetTimerRemaining(life_timer_) > 0.f)
        timer_manager->ClearTimer(life_timer_);
}

void Bullet::PhysicsTick(float delta_time)
{
    PooledObject::PhysicsTick(delta_time);

    std::vector<Actor*> out_actors;
    bool is_hit = Physics2D::OverlapCircleAll(GetTransform()->GetPosition(), .2, out_actors, static_cast<Type::uint16>(ActorLayer::kMob));
    if (is_hit)
    {
        CharacterBase* result = nullptr;
        for (Actor* actor : out_actors)
        {
            CharacterBase* character = static_cast<CharacterBase*>(actor);
            if (IsValid(character) && !character->IsDead())
            {
                result = character;
                break;
            }
        }
        
        if (IsValid(result))
        {
            Bounds bounds = {GetTransform()->GetPosition(), {.2, .2}};
            Bounds character_bounds = result->GetCollider()->GetBounds();
            Bounds intersect_bounds = Bounds::Intersect(bounds, character_bounds);

            float x = Math::RandRange(-intersect_bounds.extents.x, intersect_bounds.extents.x);
            float y = Math::RandRange(-intersect_bounds.extents.y, intersect_bounds.extents.y);

            PooledObject* hit_effect = hit_effect_pool_->SpawnPooledObject();
            if (hit_effect)
            {
                hit_effect->GetTransform()->SetPosition(intersect_bounds.center + Math::Vector2(x, y));
            }
            
            GameplayStatics::ApplyDamage(result, 10.f, GetInstigator(), GetOwner());
            Deactivate();
        }
    }
}

void Bullet::OnEnable()
{
    PooledObject::OnEnable();

    TimerManager::Get()->SetTimer(life_timer_, this, &Bullet::Deactivate, 1.f);

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
