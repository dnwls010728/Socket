#include "pch.h"
#include "UIImage.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIImage::UIImage(const std::wstring& name) :
    UIElement(name),
    ui_sprite_(nullptr),
    filter_mode_(D2D1_BITMAP_INTERPOLATION_MODE_LINEAR)
{
}

void UIImage::Render()
{
    UIElement::Render();
    if (!ui_sprite_) return;

    Renderer::Get()->DrawBitmap(ui_sprite_->GetTexture(), GetAbsolutePosition(), size_, filter_mode_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIImage>("UIImage")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
