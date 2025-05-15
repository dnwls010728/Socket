#include "pch.h"
#include "UIElement.h"

#include "Math/Rect.h"

UIElement::UIElement() :
    position_(Math::Vector2::Zero()),
    size_(Math::Vector2::Zero())
{
}

UIElement::UIElement(const Math::Vector2& position, const Math::Vector2& size) :
    position_(position),
    size_(size)
{
}

bool UIElement::IsInRange(const Math::Vector2& position) const
{
    Math::Rect rect = {
        position_.x, position_.y,
        size_.x, size_.y
    };

    return Math::Rect::Contains(rect, position);
}

UI::MouseEventResult UIElement::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    return { false, UI::CursorState::kIdle };
}

UI::MouseEventResult UIElement::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    return { false, UI::CursorState::kIdle };
}

bool UIElement::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    return false;
}

bool UIElement::OnKey(uint16_t key_code, bool is_pressed)
{
    return false;
}

bool UIElement::OnChar(wchar_t character)
{
    return false;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIElement>("UIElement")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
