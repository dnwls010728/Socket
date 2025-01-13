#include "pch.h"
#include "Block.h"

#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

Block::Block(const std::wstring& kName) :
    Actor(kName)
{
    SetLayer(ActorLayer::kBlock);

    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Tilesets\\TX Plant.png");

    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");
    renderer_->SetSprite(sprite_, L"TX Plant_0");

    collider_ = AddComponent<CircleColliderComponent>(L"Collider");
    collider_->SetRadius(.2f);
}

ColliderComponent* Block::GetCollider() const
{
    return collider_;
}

void Block::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    renderer_->SetZOrder(static_cast<int>(std::round(GetTransform()->GetPosition().y)) * -1.f);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Block>("Block")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
