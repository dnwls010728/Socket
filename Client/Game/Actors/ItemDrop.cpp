#include "pch.h"
#include "ItemDrop.h"

#include <numbers>

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Characters/Components/Controller2DComponent.h"
#include "Windows/DX/Sprite.h"

ItemDrop::ItemDrop(const std::wstring& name) :
    NetworkActor(name),
    gravity_(-20.f),
    base_y_(0.f),
    moved_(0.f),
    velocity_(Math::Vector2::Zero()),
    current_state_(State::kDropped)
{
    SetLayer(ActorLayer::kItemDrop);
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\101.png");
    if (sprite) renderer_->SetSprite(sprite, L"101_0");

    renderer_->SetZOrder(1001);
    
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetSize({ .5f, .5f });
    
    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");

    velocity_.y = 5.f;
    
}

void ItemDrop::Tick(float delta_time)
{
    NetworkActor::Tick(delta_time);
    
    const Controller2DComponent::CollisionInfo& collisions = controller_->GetCollisions();
    
    switch (current_state_)
    {
    case State::kDropped:
        {
            float angle = GetTransform()->GetAngle();
            angle += 360.f * delta_time;
            GetTransform()->SetAngle(angle);
            
            velocity_.y += gravity_ * delta_time;
            controller_->Move(velocity_ * delta_time);

            if (collisions.is_below)
            {
                current_state_ = State::kFloating;
                base_y_ = GetTransform()->GetPosition().y;
                GetTransform()->SetAngle(0.f);
            }
        }
        break;

    case State::kFloating:
        {
            Math::Vector2 position = GetTransform()->GetPosition();
            position.y = base_y_ + .25f + (std::cos(moved_ + std::numbers::pi_v<float>) - 1.f) * .125f;
            GetTransform()->SetPosition(position);
            
            moved_ = (moved_ < 360.f) ? moved_ + .025f : 0.f;
        }
        break;
        
    }
    
    if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ItemDrop>("ItemDrop")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
