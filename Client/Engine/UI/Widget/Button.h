#pragma once
#include "UI/Widget.h"

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

protected:
    virtual void Render() override;
    
private:
    UITexture* texture_;

    DrawMode draw_mode_;
    
};
