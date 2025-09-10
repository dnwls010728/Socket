#include "pch.h"
#include "Block.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Windows/DX/Sprite.h"

Block::Block(const std::wstring& name) :
    NetworkActor(name)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Block.png");
    if (sprite) renderer_->SetSprite(sprite);
}

void Block::Init(const std::wstring& color, const Math::Vector2& position) const
{
    renderer_->SetColor(Math::Color::HexToColor(color));
    GetTransform()->SetPosition(position);
}

void Block::OnActivate()
{
    if (HasBegunPlay()) SetActive(true);
}

void Block::OnDeactivate()
{
    SetActive(false);
}

void Block::OnEnable()
{
    NetworkActor::OnEnable();
    NetworkSubsystem::Get()->RegisterNetworkActor(GetSharedThis());
}

void Block::OnDisable()
{
    NetworkActor::OnDisable();
    NetworkSubsystem::Get()->UnregisterNetworkActor(GetSharedThis());
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
