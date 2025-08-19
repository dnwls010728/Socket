#include "pch.h"
#include "Slider.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

Slider::Slider(const std::wstring& name) : 
    UIContainer(name),
    ratio_(0.f),
    step_(0.f),
    min_(0.f),
    max_(1.f),
    value_changed_event_([](float value){})
{
}

float Slider::GetValue() const
{
    return min_ + (max_ - min_) * ratio_;
}

void Slider::SetRange(float min, float max)
{
    min = Math::Min(min, max);
    max = Math::Max(min, max);
}

void Slider::SetValue(float value)
{
    value = Math::Clamp(value, min_, max_);

    if (step_ > 0.f)
    {
        float step_count = std::round((value - min_) / step_);
        value = min_ + step_ * step_count;
        value = Math::Clamp(value, min_, max_);
    }

    value_changed_event_(value);
    ratio_ = (value - min_) / (max_ - min_);
}

void Slider::OnValueChanged(void(* func)(float))
{
    value_changed_event_ = func;
}

void Slider::Render()
{
    Renderer* renderer = Renderer::Get();
    UIContainer::Render();

    Math::Vector2 absolute_position = GetAbsolutePosition();
    Math::Vector2 size = GetSize();

    Math::Vector2 bar_position = absolute_position + Math::Vector2(0.f, size.y * .5f - 5.f);
    renderer->DrawSolidRoundBox(bar_position, {size.x, 10.f}, {58, 65, 74, 230}, 4.f);
    
    float width = GetSize().x * ratio_;
    Math::Vector2 thumb_position = absolute_position + Math::Vector2(width, size.y * .5f);
    renderer->DrawSolidCircle(thumb_position, size.y * .5f, {255, 211, 77, 242});
}

bool Slider::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    
    if (button == MouseButton::kLeft && is_pressed)
    {
        Math::Vector2 absolute_position = GetAbsolutePosition();
    
        float dx = position.x - absolute_position.x;
        float raw_ratio = Math::Clamp01(dx / GetSize().x);
    
        float raw_value = min_ + (max_ - min_) * raw_ratio;
        SetValue(raw_value);
        return true;
    }
    
    return false;
}

bool Slider::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool Slider::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    Math::Vector2 absolute_position = GetAbsolutePosition();
    
    float dx = position.x - absolute_position.x;
    float raw_ratio = Math::Clamp01(dx / GetSize().x);
    
    float raw_value = min_ + (max_ - min_) * raw_ratio;
    SetValue(raw_value);
    
    return true;
}

bool Slider::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Slider>("Slider")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
