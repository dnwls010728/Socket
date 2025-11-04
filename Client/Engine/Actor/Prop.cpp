#include "pch.h"
#include "Prop.h"

#include "Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"

Prop::Prop(const std::wstring& name) :
    Actor(name)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
}

void Prop::SetSprite(const std::wstring& path, int32_t frame_index) const
{
    Sprite* sprite = AssetManager::Get()->Load<Sprite>(path);
    if (!sprite) return;
    
    renderer_->SetSprite(sprite, frame_index);
}

void Prop::Render(float alpha)
{
    Actor::Render(alpha);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Prop>("Prop")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
