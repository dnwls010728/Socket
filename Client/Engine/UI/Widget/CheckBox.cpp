#include "pch.h"
#include "CheckBox.h"

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::CheckBox::CheckBox(const std::wstring& kName) :
    Widget(kName),
    is_checked_(false),
    check_event([&](bool is_checked) {})
{
}

void UI::CheckBox::OnClick(void(* func)(bool))
{
    check_event = func;
}

void UI::CheckBox::SetCheck(bool value)
{
    is_checked_ = value;
}

std::shared_ptr<UI::CheckBox> UI::CheckBox::Create(const std::wstring& kName)
{
    return std::make_shared<CheckBox>(kName);
}

void UI::CheckBox::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    const Math::Rect kRect = GetRect();

    renderer->DrawBox(window, kRect, GetPivotPosition(), Math::Color::Black, 0.f);
    if (is_checked_)
    {
        renderer->DrawSolidBox(window, { kRect.x + 2.f, kRect.y + 2.f, kRect.width - 4.f, kRect.height - 4.f }, GetPivotPosition(), Math::Color::White, 0.f);
    }
}

bool UI::CheckBox::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp)
{
    Widget::OnMouseButton(kPosition, button, is_pressed, timestamp);
    if (button == MouseButton::kLeft && is_pressed)
    {
        is_checked_ = !is_checked_;
        check_event(is_checked_);
        return true;
    }
    
    return false;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::CheckBox>("UI::CheckBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
