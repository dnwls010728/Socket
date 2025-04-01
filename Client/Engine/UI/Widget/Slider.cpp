#include "pch.h"
#include "Slider.h"

#include "Math/Color.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UI::Slider::Slider(const std::wstring& kName) :
    Widget(kName),
    value_(0.f),
    min_value_(20.f),
    max_value_(100.f),
    step_(1.f),
    value_changed_event_([&](float value) {})
{
}

void UI::Slider::OnSlide(void(* func)(float))
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
    
    const Math::Rect kRect = GetRect();

    float ratio = (value_ - min_value_) / (max_value_ - min_value_);
    ratio = Math::Clamp(ratio, 0.f, 1.f);
    // ratio = Math::Clamp(ratio, 0.f, 1.f);
    renderer->DrawSolidBox(window, {kRect.x, kRect.y, kRect.width * ratio, kRect.height}, GetPivotPosition(), Math::Color::White, 0.f);
    renderer->DrawBox(window, kRect, GetPivotPosition(), Math::Color::Black, 0.f);
}

bool UI::Slider::OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed)
{
    Widget::OnMouseButton(kPosition, button, is_pressed);

    if (button == MouseButton::kLeft && is_pressed)
    {
        SetValueInternal(kPosition);
        return true;
    }
    
    return false;
}

bool UI::Slider::OnDragBegin(const Math::Vector2& kPosition)
{
    return true;
}

bool UI::Slider::OnDragEnd(const Math::Vector2& kPosition)
{
    return true;
}

bool UI::Slider::OnDrag(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    SetValueInternal(kPosition);
    return true;
}

void UI::Slider::SetValueInternal(const Math::Vector2& kPosition)
{
    const Math::Rect kRect = GetRect();
    
    const float x = kPosition.x - kRect.x;
    const float y = kPosition.y - kRect.y;

    float value = x / kRect.width;
    value = Math::Clamp(value, 0.f, 1.f);

    float raw_value = value * (max_value_ - min_value_) + min_value_;
    float snapped_value = std::round(raw_value / step_) * step_;

    SetValue(snapped_value);
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
