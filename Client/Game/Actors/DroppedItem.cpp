#include "pch.h"
#include "DroppedItem.h"

#include <numbers>

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Characters/Components/Controller2DComponent.h"
#include "Math/Math.h"
#include "Windows/DX/Sprite.h"

DroppedItem::DroppedItem(const std::wstring& name) :
    NetworkActor(name),
    current_state_(State::kDropped),
    start_position_(Math::Vector2::Zero()),
    drop_position_(Math::Vector2::Zero()),
    velocity_(Math::Vector2::Zero()),
    timer_(0.f)
{
    SetLayer(ActorLayer::kItemDrop);
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(1001);

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\101.png");
    if (sprite) renderer_->SetSprite(sprite, L"101_0");

    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetSize({ 1.f, 1.f });

    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");
    
}

void DroppedItem::OnActivate()
{
    if (HasBegunPlay()) SetActive(true);
}

void DroppedItem::OnDeactivate()
{
    SetActive(false);
}

void DroppedItem::Init(uint32_t item_id, const Math::Vector2& drop_position)
{
    drop_position_ = drop_position;
}

void DroppedItem::BeginPlay()
{
    NetworkActor::BeginPlay();
    
}

void DroppedItem::Tick(float delta_time)
{
    NetworkActor::Tick(delta_time);
    
    Math::Vector2 position = GetTransform()->GetPosition();

    if (timer_ < 1.f)
    {
        timer_ += delta_time;

        float distance = drop_position_.x - position.x;
        velocity_.x = distance / (1.f - timer_);
        GetTransform()->Translate(velocity_ * delta_time);

        float angle = Math::Lerp(0.f, 360.f, timer_ / 1.f);
        GetTransform()->SetAngle(angle);
    }
    else velocity_.x = 0.f;

}


void DroppedItem::OnEnable()
{
    NetworkActor::OnEnable();

    Math::Vector2 position = GetTransform()->GetPosition();
    start_position_ = position;

    timer_ = 0.f;

}

void DroppedItem::OnDisable()
{
    NetworkActor::OnDisable();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<DroppedItem>("DroppedItem")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
