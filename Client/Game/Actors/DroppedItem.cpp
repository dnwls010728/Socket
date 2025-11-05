#include "pch.h"
#include "DroppedItem.h"

#include <numbers>

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"
#include "Windows/DX/Sprite.h"

DroppedItem::DroppedItem(const std::wstring& name) :
    NetworkActor(name),
    is_color_(false)
{
    SetLayer(ActorLayer::kDroppedItem);
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(std::numeric_limits<int32_t>::max());

    animator_ = AddComponent<AnimatorComponent>(L"Animator");

    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Color.png.apack");
    if (animation_pack) animator_->SetAnimationPack(animation_pack);

    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    collider_->SetOffset({ 0.f, 0.f });
    collider_->SetSize({ 1.f, 1.f });
    collider_->SetTrigger(true);
    
}

void DroppedItem::Pickup(const std::shared_ptr<Actor>& character)
{
    ObjectPoolSubsystem::Get()->ReturnToPool(GetSharedThis());
}

void DroppedItem::OnActivate()
{
    if (HasBegunPlay()) SetActive(true);
}

void DroppedItem::OnDeactivate()
{
    is_color_ = false;
    animator_->StopAnimation();
    SetActive(false);
}

void DroppedItem::Init(uint32_t item_id, int32_t color, const Math::Vector2& drop_position)
{
    if (color)
    {
        if (color > 999) animator_->PlayAnimation(L"Y"); // 1000원 이상
        else if (color > 99) animator_->PlayAnimation(L"M"); // 100원 이상
        else animator_->PlayAnimation(L"C");

        is_color_ = true;
        return;
    }

    const ItemData* item_data = DataSubsystem::Get()->GetItem(item_id);
    if (!item_data) return;
    
    Sprite* sprite = AssetManager::Get()->Load<Sprite>(item_data->icon.path);
    int32_t frame_index = item_data->icon.index;
    
    if (!sprite)
    {
        static Sprite* kMissing = AssetManager::Get()->Load<Sprite>(L"Sprites\\Missing.png");
        sprite = kMissing;
        frame_index = 0;
    }
    
    if (sprite) renderer_->SetSprite(sprite, frame_index);
}


void DroppedItem::OnEnable()
{
    NetworkActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
}

void DroppedItem::OnDisable()
{
    NetworkActor::OnDisable();
    NetworkSubsystem::Get()->UnregisterNetworkActor(GetSharedThis());
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
