#include "pch.h"
#include "Widget.h"

#include "UIManager.h"
#include "Input/Keyboard.h"
#include "Windows/DX/Renderer.h"

UI_OLD::Widget::Widget(const std::wstring& kName) :
    name_(kName),
    position_(Math::Vector2::Zero()),
    size_(Math::Vector2::One()),
    pivot_({.5f, .5f}),
    is_focused_(false)
{
}

void UI_OLD::Widget::AddToViewport()
{
    Manager* manager = Manager::Get();

    manager->widgets_.push_back(GetSharedThis());
    OnAdd();
}

void UI_OLD::Widget::RemoveFromViewport()
{
    Manager* manager = Manager::Get();

    OnRemove();
    std::erase(manager->widgets_, GetSharedThis());
}

Math::Rect UI_OLD::Widget::GetRect() const
{
    return GetRect(position_, size_, pivot_);
}

Math::Vector2 UI_OLD::Widget::GetPivotPosition() const
{
    return GetPivotPosition(GetRect(), pivot_);
}

bool UI_OLD::Widget::IsInViewport()
{
    Manager* manager = Manager::Get();
    
    const std::vector<std::shared_ptr<Widget>>& widgets_ = manager->widgets_;
    return std::ranges::find(widgets_, GetSharedThis()) != widgets_.end();
}

bool UI_OLD::Widget::Contains(const Math::Vector2& kPosition) const
{
    return Math::Rect::Contains(GetRect(), kPosition);
}

std::shared_ptr<UI_OLD::Widget> UI_OLD::Widget::Create(const std::wstring& kName)
{
    return std::make_shared<Widget>(kName);
}

Math::Rect UI_OLD::Widget::GetRect(const Math::Vector2& kPosition, const Math::Vector2& kSize, const Math::Vector2& kPivot)
{
    float x = kPosition.x - (kSize.x * kPivot.x);
    float y = kPosition.y - (kSize.y * (1.f - kPivot.y));
    return {x, y, kSize.x, kSize.y};
}

Math::Vector2 UI_OLD::Widget::GetPivotPosition(const Math::Rect& kRect, const Math::Vector2& kPivot)
{
    return {kRect.x + kRect.width * kPivot.x, kRect.y + kRect.height * (1.f - kPivot.y)};
}

void UI_OLD::Widget::Tick(float delta_time)
{
}

void UI_OLD::Widget::Render(Renderer* renderer, WindowsWindow* window)
{
}

void UI_OLD::Widget::OnAdd()
{
}

void UI_OLD::Widget::OnRemove()
{
}

bool UI_OLD::Widget::OnMouseEnter()
{
    return false;
}

bool UI_OLD::Widget::OnMouseLeave()
{
    return false;
}

bool UI_OLD::Widget::OnMouseMotion(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI_OLD::Widget::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp)
{
    if (button == MouseButton::kLeft && is_pressed && !IsFocused()) Manager::Get()->SetFocus(GetSharedThis());
    return false;
}

bool UI_OLD::Widget::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI_OLD::Widget::OnDragBegin(const Math::Vector2& kPosition)
{
    return false;
}

bool UI_OLD::Widget::OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
}

bool UI_OLD::Widget::OnDragEnd(const Math::Vector2& kPosition)
{
    return false;
}

bool UI_OLD::Widget::OnDrop(const std::shared_ptr<Widget>& kWidget, const Math::Vector2& kPosition)
{
    return false;
}

bool UI_OLD::Widget::OnKey(uint16_t key_code, bool is_pressed)
{
    return false;
}

bool UI_OLD::Widget::OnChar(wchar_t character)
{
    return false;
}

void UI_OLD::Widget::OnFocus(bool is_focus)
{
    is_focused_ = is_focus;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI_OLD::Widget>("UI::Widget")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
