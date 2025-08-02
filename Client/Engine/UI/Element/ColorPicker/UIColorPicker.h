#pragma once
#include "UIHueSlider.h"
#include "UIPalette.h"
#include "Math/Color.h"
#include "UI/UIContainer.h"

class UIColorPicker : public UIContainer
{
    GENERATED_BODY(UIColorPicker, UIContainer)
    
public:
    UIColorPicker(const std::wstring& name);
    virtual ~UIColorPicker() override = default;

    const Math::Color& GetColor() const;
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(Math::Color)>, std::decay_t<F>>>>
    void OnValueChanged(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(Math::Color));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(Math::Color) const);

    void OnValueChanged(void(*func)(Math::Color));

private:
    void OnColorChanged(Math::Color color);
    void OnHueChanged(float value) const;

    UIPalette* palette_;
    UIHueSlider* hue_slider_;

    Function<void(Math::Color)> value_changed_event_;
    
};

template <typename F, typename>
void UIColorPicker::OnValueChanged(F&& func)
{
    value_changed_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIColorPicker::OnValueChanged(M* target, void(M::* func)(Math::Color))
{
    value_changed_event_ = {target, func};
}

template <typename M, typename>
void UIColorPicker::OnValueChanged(M* target, void(M::* func)(Math::Color) const)
{
    value_changed_event_ = {target, func};
}
