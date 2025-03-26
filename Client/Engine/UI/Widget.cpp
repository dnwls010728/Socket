#include "pch.h"
#include "Widget.h"

#include "Input/Keyboard.h"
#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::Widget::Widget(const std::wstring& kName) :
    name_(kName),
    position_(Math::Vector2::Zero()),
    size_(Math::Vector2::One())
{
}

Math::Rect UI::Widget::GetRect() const
{
    return {position_.x, position_.y, size_.x, size_.y};
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
    return false;
}

bool UI::Widget::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    return false;
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
