#pragma once
#include "UI/UIContainer.h"

class UISlider : public UIContainer
{
    GENERATED_BODY(UISlider, UIContainer)
    
public:
    UISlider(const std::wstring& name);
    virtual ~UISlider() override = default;

    float GetValue() const;

    void SetRange(float min, float max);
    void SetValue(float value);
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(float)>, std::decay_t<F>>>>
    void OnValueChanged(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(float));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(float) const);

    void OnValueChanged(void(*func)(float));

    FORCEINLINE void SetStep(float step) { step_ = step; }

protected:
    virtual void Render() override;

    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    float ratio_;
    float step_;
    float min_;
    float max_;

    Function<void(float)> value_changed_event_;
    
};

template <typename F, typename>
void UISlider::OnValueChanged(F&& func)
{
    value_changed_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UISlider::OnValueChanged(M* target, void(M::* func)(float))
{
    value_changed_event_ = { target, func };
}

template <typename M, typename>
void UISlider::OnValueChanged(M* target, void(M::* func)(float) const)
{
    value_changed_event_ = { target, func };
}
