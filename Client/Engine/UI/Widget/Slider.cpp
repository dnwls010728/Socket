#include "pch.h"
#include "Slider.h"

#include "Math/Color.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UI::Slider::Slider(const std::wstring& kName) :
    Widget(kName),
    value_(0.f),
    min_value_(0.f),
    max_value_(100.f),
    value_changed_event_([&](float value) {})
{
}

void UI::Slider::AddValueChangedEvent(void(* func)(float))
{
    value_changed_event_ = func;
}

void UI::Slider::SetValue(float value)
{
    value_ = value;
    value_changed_event_(value_);
}

std::shared_ptr<UI::Slider> UI::Slider::Create(const std::wstring& kName)
{
    return std::make_shared<Slider>(kName);
}

void UI::Slider::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    renderer->DrawBox(window, GetRect(), GetPivotPosition(), Math::Color::Black, 0.f);

    const Math::Rect kRect = GetRect();
    float ratio = (value_ - min_value_) / (max_value_ - min_value_);
    ratio = Math::Clamp(ratio, 0.f, 1.f);
    renderer->DrawSolidBox(window, {kRect.x, kRect.y, kRect.width * ratio, kRect.height}, GetPivotPosition(), Math::Color::White, 0.f);

    std::wstring text = std::to_wstring(value_);
    renderer->DrawString(window, text, GetRect(), GetPivotPosition(), Math::Color::Black, 0.f, L"Nanum18");
}

bool UI::Slider::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    Widget::OnMouseButton(kPosition, button, is_pressed);

    if (button == MouseButton::kLeft && is_pressed)
    {
        const Math::Rect rect = GetRect();
        const float x = kPosition.x - rect.x;
        const float y = kPosition.y - rect.y;
        const float value = x / rect.width;
        SetValue(value * (max_value_ - min_value_) + min_value_);
        return true;
    }
    
    return false;
}

bool UI::Slider::OnDrag(const Math::Vector2& kPosition)
{
    const Math::Rect rect = GetRect();
    const float x = kPosition.x - rect.x;
    const float y = kPosition.y - rect.y;
    float value = x / rect.width;
    value = Math::Clamp(value, 0.f, 1.f);
    SetValue(value * (max_value_ - min_value_) + min_value_);
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Slider>("UI::Slider")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
