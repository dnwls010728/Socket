#include "pch.h"
#include "Button.h"

#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UI::Button::Button(const std::wstring& kName) :
    Widget(kName),
    click_event([]() {}),
    text_(L"Text")
{
}

void UI::Button::OnClick(void(* func)())
{
    click_event = func;
}

std::shared_ptr<UI::Button> UI::Button::Create(const std::wstring& kName)
{
    return std::make_shared<Button>(kName);
}

void UI::Button::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    const Math::Rect rect = GetRect();

    renderer->DrawSolidBox(window, rect, GetPivotPosition(), {0, 0, 0, 100});
    renderer->DrawBox(window, rect, GetPivotPosition(), Math::Color::White);
    renderer->DrawString(window, text_, rect, GetPivotPosition(), Math::Color::White, 0.f, L"NanumBarunGothic", 12.f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

bool UI::Button::OnMouseEnter()
{
    return true;
}

bool UI::Button::OnMouseLeave()
{
    return true;
}

bool UI::Button::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed, double timestamp)
{
    Widget::OnMouseButton(kPosition, button, is_pressed, timestamp);
    if (button == MouseButton::kLeft && is_pressed)
    {
        click_event();
        return true;
    }
    
    return false;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Button>("UI::Button")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
