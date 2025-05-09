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
    is_focused_(false)
{
}

Math::Rect UI::Widget::GetRect() const
{
    return GetRect(position_, size_, pivot_);
}

Math::Vector2 UI::Widget::GetPivotPosition() const
{
    return GetPivotPosition(GetRect(), pivot_);
}

bool UI::Widget::Contains(const Math::Vector2& kPosition) const
{
    return Math::Rect::Contains(GetRect(), kPosition);
}

std::shared_ptr<UI::Widget> UI::Widget::Create(const std::wstring& kName)
{
    return std::make_shared<Widget>(kName);
}

Math::Rect UI::Widget::GetRect(const Math::Vector2& kPosition, const Math::Vector2& kSize, const Math::Vector2& kPivot)
{
    float x = kPosition.x - (kSize.x * kPivot.x);
    float y = kPosition.y - (kSize.y * (1.f - kPivot.y));
    return {x, y, kSize.x, kSize.y};
}

Math::Vector2 UI::Widget::GetPivotPosition(const Math::Rect& kRect, const Math::Vector2& kPivot)
{
    return {kRect.x + kRect.width * kPivot.x, kRect.y + kRect.height * (1.f - kPivot.y)};
}

void UI::Widget::Tick(float delta_time)
{
}

void UI::Widget::Render(Renderer* renderer, WindowsWindow* window)
{
}

void UI::Widget::OnAdd()
{
}

void UI::Widget::OnRemove()
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

bool UI::Widget::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp)
{
    if (button == MouseButton::kLeft && is_pressed && !IsFocused()) Manager::Get()->SetFocus(GetSharedThis());
    return false;
}

bool UI::Widget::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI::Widget::OnDragBegin(const Math::Vector2& kPosition)
{
    return false;
}

bool UI::Widget::OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI::Widget::OnDragEnd(const Math::Vector2& kPosition)
{
    return false;
}

bool UI::Widget::OnDrop(const std::shared_ptr<Widget>& kWidget, const Math::Vector2& kPosition)
{
    return false;
}

bool UI::Widget::OnKey(uint16_t key_code, bool is_pressed)
{
    return false;
}

bool UI::Widget::OnChar(wchar_t character)
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
