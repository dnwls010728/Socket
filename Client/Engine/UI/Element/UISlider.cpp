#include "pch.h"
#include "UISlider.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UISlider::UISlider(const std::wstring& name) : 
    UIContainer(name),
    ratio_(0.f),
    step_(0.f),
    min_(0.f),
    max_(1.f),
    value_changed_event_([](float value){})
{
}

float UISlider::GetValue() const
{
    return min_ + (max_ - min_) * ratio_;
}

void UISlider::SetRange(float min, float max)
{
    min_ = Math::Min(min, max);
    max_ = Math::Max(min, max);
}

void UISlider::SetValue(float value)
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

void UISlider::Render()
{
    Renderer* renderer = Renderer::Get();
    UIContainer::Render();

    Math::Vector2 absolute_position = GetAbsolutePosition();
    Math::Vector2 size = GetSize();
    
    float fill_width = size.x * ratio_;
    Math::Vector2 thumb_position = absolute_position + Math::Vector2(fill_width, size.y * .5f);
    renderer->DrawSolidCircle(thumb_position, size.y * .5f, {255, 211, 77, 242});
}

bool UISlider::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
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

bool UISlider::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UISlider::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    Math::Vector2 absolute_position = GetAbsolutePosition();
    
    float dx = position.x - absolute_position.x;
    float raw_ratio = Math::Clamp01(dx / GetSize().x);
    
    float raw_value = min_ + (max_ - min_) * raw_ratio;
    SetValue(raw_value);
    
    return true;
}

bool UISlider::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UISlider>("UISlider")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
