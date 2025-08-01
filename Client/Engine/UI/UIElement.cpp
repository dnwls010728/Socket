#include "pch.h"
#include "UIElement.h"

#include "UIContainer.h"
#include "UIState.h"
#include "Math/Rect.h"

bool UIElement::IsInRange(const Math::Vector2& position) const
{
    Math::Vector2 parent_position = parent_ ? parent_->GetAbsolutePosition() : Math::Vector2::Zero();
    Math::Rect rect = {
        parent_position.x + relative_position_.x, parent_position.y + relative_position_.y,
        GetSize().x, GetSize().y
    };

    return !is_ignore_raycast ? Math::Rect::Contains(rect, position) : false;
}

void UIElement::SetAbsolutePosition(const Math::Vector2& position)
{
    if (parent_)
    {
        Math::Vector2 parent_position = parent_->GetAbsolutePosition();
        relative_position_ = position - parent_position;
    }
    else relative_position_ = position;
    MakeDirty();
}

const Math::Vector2& UIElement::GetAbsolutePosition()
{
    if (is_dirty_) UpdateAbsolutePosition();
    return absolute_position_;
}

void UIElement::SetRelativePosition(const Math::Vector2& position)
{
    relative_position_ = position;
    MakeDirty();
}

bool UIElement::IsDescendantOf(UIElement* ancestor) const
{
    UIElement *parent = parent_;
    while (parent)
    {
        if (parent == ancestor)
            return true;
        else
            parent = parent->parent_;
    }
    return false;
}

UIElement::UIElement(const std::wstring& name) :
    name_(name),
    has_initialized_(false),
    is_active_(true),
    is_focused_(false),
    is_ignore_raycast(false),
    parent_(nullptr),
    relative_position_(Math::Vector2::Zero()),
    absolute_position_(Math::Vector2::Zero()),
    size_(Math::Vector2::Zero())
{
}

void UIElement::Init()
{
    has_initialized_ = true;
}

UIElement* UIElement::RayCast(const Math::Vector2& position)
{
    return IsInRange(position) ? this : nullptr;
}

UI::MouseEventResult UIElement::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    return { false, UI::CursorState::kIdle };
}

UI::MouseEventResult UIElement::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    if (button == MouseButton::kLeft && is_pressed && !is_focused_)
        UI::Get()->GetState()->UpdateFocus(this);
    
    return { false, UI::CursorState::kIdle };
}

bool UIElement::OnMouseEnter()
{
    return false;
}

bool UIElement::OnMouseLeave()
{
    return false;
}

bool UIElement::OnDragBegin(const Math::Vector2& position)
{
    return false;
}

bool UIElement::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    return false;
}

bool UIElement::OnDragEnd(const Math::Vector2& position)
{
    return false;
}

bool UIElement::OnDrop(const Math::Vector2& position, UIElement* target)
{
    return false;
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

void UIElement::OnFocus(bool is_focused)
{
    is_focused_ = is_focused;
}

void UIElement::UpdateAbsolutePosition()
{
    if (parent_) absolute_position_ = parent_->GetAbsolutePosition() + relative_position_;
    else absolute_position_ = relative_position_;
    
    is_dirty_ = false;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIElement>("UIElement")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
