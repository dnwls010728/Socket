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
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(const Math::Color&)>, std::decay_t<F>>>>
    void OnValueChanged(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(const Math::Color&));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(const Math::Color&) const);

    void OnValueChanged(void(*func)(const Math::Color&));

private:
    void OnColorChanged(const Math::Color& color);
    void OnHueChanged(float value) const;

    UIHueSlider* hue_slider_;
    UIPalette* palette_;

    Function<void(const Math::Color&)> value_changed_event_;
    
};

template <typename F, typename>
void UIColorPicker::OnValueChanged(F&& func)
{
    value_changed_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIColorPicker::OnValueChanged(M* target, void(M::* func)(const Math::Color&))
{
    value_changed_event_ = {target, func};
}

template <typename M, typename>
void UIColorPicker::OnValueChanged(M* target, void(M::* func)(const Math::Color&) const)
{
    value_changed_event_ = {target, func};
}
