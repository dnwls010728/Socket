#include "pch.h"
#include "UIImage.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UIImage::UIImage() :
    texture_(nullptr),
    filter_mode_(D2D1_BITMAP_INTERPOLATION_MODE_LINEAR)
{
}

void UIImage::Render()
{
    UIElement::Render();
    if (!texture_) return;

    Renderer::Get()->DrawBitmap(texture_->GetTexture(), GetAbsolutePosition(), size_, filter_mode_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIImage>("UIImage")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
