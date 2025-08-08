#include "pch.h"
#include "UIColorPicker.h"

#include "Windows/DX/Renderer.h"

UIColorPicker::UIColorPicker(const std::wstring& name) :
    UIContainer(name),
    value_changed_event_([](Math::Color color) {})
{
    SetSize({240.f, 200.f});

    hue_slider_ = AddChild<UIHueSlider>(UIHueSlider::StaticClass(), L"HueSlider");
    hue_slider_->SetRelativePosition({210.f, 0.f});
    hue_slider_->OnValueChanged(this, &UIColorPicker::OnHueChanged);
    
    palette_ = AddChild<UIPalette>(UIPalette::StaticClass(), L"Palette");
    palette_->OnValueChanged(this, &UIColorPicker::OnColorChanged);
}

const Math::Color& UIColorPicker::GetColor() const
{
    return palette_->GetColor();
}

void UIColorPicker::OnValueChanged(void(* func)(const Math::Color&))
{
    value_changed_event_ = func;
}

void UIColorPicker::OnColorChanged(const Math::Color& color)
{
    value_changed_event_(color);
}

void UIColorPicker::OnHueChanged(float value) const
{
    palette_->SetHue(value * 360.f);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIColorPicker>("UIColorPicker")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
