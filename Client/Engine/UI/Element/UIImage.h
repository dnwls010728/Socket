#pragma once
#include <d2d1.h>

#include "UI/UIElement.h"

class UISprite;

class UIImage : public UIElement
{
    GENERATED_BODY(UIImage, UIElement)
    
public:
    UIImage(const std::wstring& name);
    virtual ~UIImage() override = default;

    FORCEINLINE void SetTexture(UISprite* ui_sprite) { ui_sprite_ = ui_sprite; }

    FORCEINLINE void SetFilterMode(const D2D1_BITMAP_INTERPOLATION_MODE mode) { filter_mode_ = mode; }
    FORCEINLINE D2D1_BITMAP_INTERPOLATION_MODE GetFilterMode() const { return filter_mode_; }

protected:
    virtual void Render() override;

    UISprite* ui_sprite_;

    D2D1_BITMAP_INTERPOLATION_MODE filter_mode_;
    
};
