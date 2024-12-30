#pragma once
#include "Math/Color.h"
#include "UI_OLD/Widget.h"

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

    FORCEINLINE void SetColor(const Math::Color& color) { color_ = color; }
    FORCEINLINE const Math::Color& GetColor() const { return color_; }

protected:
    virtual void Render() override;

private:
    UITexture* texture_;

    DrawMode draw_mode_;

    Math::Color color_;
    
};
