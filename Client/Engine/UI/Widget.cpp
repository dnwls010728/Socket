#include "pch.h"
#include "Widget.h"

#include "UIManager.h"
#include "Input/Keyboard.h"
#include "Windows/DX/Renderer.h"

UI::Widget::Widget(const std::wstring& kName) :
    name_(kName),
    position_(Math::Vector2::Zero()),
    size_(Math::Vector2::One()),
    pivot_({.5f, .5f}),
    angle_(0.f),
    is_focused_(false)
{
}

Math::Rect UI::Widget::GetRect() const
{
    float x = position_.x - size_.x * pivot_.x;
    float y = position_.y - size_.y * (1.f - pivot_.y);
    float width = size_.x;
    float height = size_.y;

    return {x, y, width, height};
}

Math::Vector2 UI::Widget::GetPivotPosition() const
{
    const Math::Rect kRect = GetRect();
    const Math::Vector2 kPivot = GetPivot();

    return {kRect.x + kRect.width * kPivot.x, kRect.y + kRect.height * (1.f - kPivot.y)};
}

bool UI::Widget::Contains(const Math::Vector2& kPosition) const
{
    return Math::Rect::Contains(GetRect(), kPosition);
}

std::shared_ptr<UI::Widget> UI::Widget::Create(const std::wstring& kName)
{
    return std::make_shared<Widget>(kName);
}

void UI::Widget::Tick(float delta_time)
{
}

void UI::Widget::Render(Renderer* renderer, WindowsWindow* window)
{
}

bool UI::Widget::OnMouseEnter()
{
    return false;
}

bool UI::Widget::OnMouseLeave()
{
    return false;
}

bool UI::Widget::OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI::Widget::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    if (button == MouseButton::kLeft && is_pressed && !IsFocused()) Manager::Get()->SetFocus(GetSharedThis());
    return false;
}

bool UI::Widget::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI::Widget::OnDragBegin()
{
    return false;
}

bool UI::Widget::OnDrag(const Math::Vector2& kPosition)
{
    return false;
}

bool UI::Widget::OnDragEnd()
{
    return false;
}

bool UI::Widget::OnDrop(const std::shared_ptr<Widget>& kWidget)
{
    return false;
}

void UI::Widget::OnFocus(bool is_focus)
{
    is_focused_ = is_focus;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Widget>("UI::Widget")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
