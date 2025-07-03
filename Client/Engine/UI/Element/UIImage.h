#pragma once
#include <d2d1.h>

#include "UI/UIElement.h"

class UISprite;

class UIImage : public UIElement
{
    GENERATED_BODY(UIImage, UIElement)
    
public:
    enum class DrawMode
    {
        kSimple,
        kSliced
    };
    
    UIImage(const std::wstring& name);
    virtual ~UIImage() override = default;

    void SetSprite(UISprite* ui_sprite, const std::wstring& frame_name);

    FORCEINLINE void SetDrawMode(DrawMode draw_mode) { draw_mode_ = draw_mode; }
    FORCEINLINE DrawMode GetDrawMode() const { return draw_mode_; }

protected:
    virtual void Render() override;

    UISprite* ui_sprite_;

    std::wstring current_frame_;

    DrawMode draw_mode_;
    
};
