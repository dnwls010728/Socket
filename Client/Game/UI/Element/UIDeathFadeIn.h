#pragma once
#include "Math/Color.h"
#include "UI/UIElement.h"

class UIDeathFadeIn : public UIElement
{
    GENERATED_BODY(UIDeathFadeIn, UIElement)
    
public:
    UIDeathFadeIn(const std::wstring& name);
    virtual ~UIDeathFadeIn() override = default;

    void Reset();

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

private:
    float fade_timer_;

    Math::Color color_;
    
};
