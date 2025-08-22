#include "pch.h"
#include "UIHueSlider.h"

#include "Asset/AssetManager.h"
#include "Math/Math.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIHueSlider::UIHueSlider(const std::wstring& name) :
    UIContainer(name),
    value_(0.f),
    value_changed_event_([](float value) {})
{
    SetSize({30.f, 200.f});

    UISprite* hue_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Hue.png");

    image_ = AddChild<UIImage>(UIImage::StaticClass(), L"Image");
    image_->SetSize(GetSize());
    image_->SetSprite(hue_sprite, L"Hue_0");
    image_->SetIgnoreRayCast(true);
}

void UIHueSlider::Render()
{
    UIContainer::Render();

    Math::Vector2 position = GetAbsolutePosition();
    position.y += value_ * GetSize().y - 5.f;
    Renderer::Get()->DrawBox(position, {GetSize().x, 10.f}, Math::Color::White, 2.f);
}

bool UIHueSlider::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIHueSlider::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    value_ = Math::Clamp01((position.y - GetAbsolutePosition().y) / GetSize().y);
    value_changed_event_(value_);
    
    return true;
}

bool UIHueSlider::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIHueSlider>("UIHueSlider")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
