#pragma once
#include "UI/Widget.h"

class UITexture;

class Image : public Widget
{
    SHADER_CLASS_HELPER(Image)
    GENERATED_BODY(Image, Widget);
    
public:
    Image(const std::wstring& kName);
    virtual ~Image() override = default;
    
    FORCEINLINE void SetTexture(UITexture* texture) { texture_ = texture; }
    FORCEINLINE void SetDrawMode(DrawMode draw_mode) { draw_mode_ = draw_mode; }

protected:
    virtual void Render() override;

private:
    UITexture* texture_;

    DrawMode draw_mode_;
    
};
