#include "pch.h"
#include "DroppedItem.h"

#include <numbers>

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Math/Math.h"
#include "Windows/DX/Sprite.h"

DroppedItem::DroppedItem(const std::wstring& name) :
    NetworkActor(name),
    start_position_(Math::Vector2::Zero()),
    drop_position_(Math::Vector2::Zero()),
    control_(Math::Vector2::Zero()),
    timer_(0.f)
{
    SetLayer(ActorLayer::kItemDrop);
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(1001);

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\101.png");
    if (sprite) renderer_->SetSprite(sprite, L"101_0");
    
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

    if (timer_ >= 1.f) return;

    timer_ += delta_time / 1.f;
    float t = Math::Clamp01(timer_);

    Math::Vector2 m1 = Math::Vector2::Lerp(start_position_, control_, t);
    Math::Vector2 m2 = Math::Vector2::Lerp(control_, drop_position_, t);
    Math::Vector2 position = Math::Vector2::Lerp(m1, m2, t);
    GetTransform()->SetPosition(position);

    float angle = Math::Lerp(0.f, 360.f, t);
    GetTransform()->SetAngle(angle);

}


void DroppedItem::OnEnable()
{
    NetworkActor::OnEnable();

    Math::Vector2 position = GetTransform()->GetPosition();
    start_position_ = position;

    float h = 2.f;
    float dy = drop_position_.y - start_position_.y;
    float p = h + std::sqrtf(h * (h - dy));
    control_.x = (start_position_.x + drop_position_.x) * .5f;
    control_.y = start_position_.y + p;

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
