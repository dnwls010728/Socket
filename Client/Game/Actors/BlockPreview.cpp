#include "pch.h"
#include "BlockPreview.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"

BlockPreview::BlockPreview(const std::wstring& name) :
    Actor(name)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(std::numeric_limits<int32_t>::max());

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Block.png");
    if (sprite) renderer_->SetSprite(sprite);

    Math::Color color = Math::Color::White;
    color.a = 128;

    renderer_->SetColor(color);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<BlockPreview>("BlockPreview")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
