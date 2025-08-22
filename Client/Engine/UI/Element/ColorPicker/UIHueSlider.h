#pragma once
#include "Misc/FunctionMacros.h"
#include "UI/UIContainer.h"

class UIImage;

class UIHueSlider : public UIContainer
{
    GENERATED_BODY(UIHueSlider, UIContainer)
    
public:
    UIHueSlider(const std::wstring& name);
    virtual ~UIHueSlider() override = default;

    DEFINE_BIND_OVERLOADS(value_changed_event_, OnValueChanged, void, float)

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
