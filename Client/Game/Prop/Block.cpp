#include "pch.h"
#include "Block.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

Block::Block(const std::wstring& kName) :
    Actor(kName)
{
    SetLayer(ActorLayer::kBlock);

    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Tilesets\\TemplateTileset.png");

    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");
    renderer_->SetSprite(sprite_, L"TemplateTileset_0");

    collider_ = AddComponent<BoxColliderComponent>(L"Collider");
}

ColliderComponent* Block::GetCollider() const
{
    return collider_;
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
