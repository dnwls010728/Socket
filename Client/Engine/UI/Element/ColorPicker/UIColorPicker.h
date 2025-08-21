#pragma once
#include "UIHueSlider.h"
#include "UIPalette.h"
#include "Math/Color.h"
#include "Misc/FunctionMacros.h"
#include "UI/UIContainer.h"

class UIColorPicker : public UIContainer
{
    GENERATED_BODY(UIColorPicker, UIContainer)
    
public:
    UIColorPicker(const std::wstring& name);
    virtual ~UIColorPicker() override = default;

    const Math::Color& GetColor() const;

    DEFINE_BIND_OVERLOADS(value_changed_event_, OnValueChanged, void, const Math::Color&)

private:
    void OnColorChanged(const Math::Color& color);
    void OnHueChanged(float value) const;

    UIHueSlider* hue_slider_;
    UIPalette* palette_;

    Function<void(const Math::Color&)> value_changed_event_;
    
};
