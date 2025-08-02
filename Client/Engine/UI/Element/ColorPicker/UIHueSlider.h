#pragma once
#include "UI/UIContainer.h"

class UIImage;

class UIHueSlider : public UIContainer
{
    GENERATED_BODY(UIHueSlider, UIContainer)
    
public:
    UIHueSlider(const std::wstring& name);
    virtual ~UIHueSlider() override = default;
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(float)>, std::decay_t<F>>>>
    void OnValueChanged(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(float));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(float) const);

    void OnValueChanged(void(*func)(float));

protected:
    virtual void Render() override;
    
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    UIImage* image_;
    
    float value_;

    Function<void(float)> value_changed_event_;
    
};

template <typename F, typename>
void UIHueSlider::OnValueChanged(F&& func)
{
    value_changed_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIHueSlider::OnValueChanged(M* target, void(M::* func)(float))
{
    value_changed_event_ = { target, func };
}

template <typename M, typename>
void UIHueSlider::OnValueChanged(M* target, void(M::* func)(float) const)
{
    value_changed_event_ = { target, func };
}
