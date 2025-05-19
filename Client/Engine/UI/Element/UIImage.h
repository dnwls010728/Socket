#pragma once
#include <d2d1.h>

#include "UI/UIElement.h"

class UITexture;

class UIImage : public UIElement
{
    GENERATED_BODY(UIImage, UIElement)
    
public:
    UIImage();
    virtual ~UIImage() override = default;

    FORCEINLINE void SetTexture(UITexture* texture) { texture_ = texture; }

    FORCEINLINE void SetFilterMode(const D2D1_BITMAP_INTERPOLATION_MODE mode) { filter_mode_ = mode; }
    FORCEINLINE D2D1_BITMAP_INTERPOLATION_MODE GetFilterMode() const { return filter_mode_; }

protected:
    virtual void Render() override;

    UITexture* texture_;

    D2D1_BITMAP_INTERPOLATION_MODE filter_mode_;
    
};
