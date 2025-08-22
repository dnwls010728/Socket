#pragma once
#include "Misc/FunctionMacros.h"
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

    FORCEINLINE void SetStep(float step) { step_ = step; }

    DEFINE_BIND_OVERLOADS(value_changed_event_, OnValueChanged, void, float)

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
