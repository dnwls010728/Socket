#pragma once
#include "UI/Widget.h"

DECLARE_DELEGATE(OnButtonEvent)

class UITexture;

class Button : public Widget
{
    SHADER_CLASS_HELPER(Button)
    GENERATED_BODY(Button, Widget)
    
public:
    Button(const std::wstring& kName);
    virtual ~Button() override = default;

    FORCEINLINE void SetTexture(UITexture* texture) { texture_ = texture; }
    FORCEINLINE void SetDrawMode(DrawMode draw_mode) { draw_mode_ = draw_mode; }

    OnButtonEvent OnClick;

protected:
    virtual void Render() override;

    virtual bool OnMouseButton(const Math::Vector2& kPosition, MouseButton button, bool is_pressed) override;
    
private:
    UITexture* texture_;

    DrawMode draw_mode_;
    
};
