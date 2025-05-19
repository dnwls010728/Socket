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

    Math::Vector2 parent_position = parent_ ? parent_->GetPosition() : Math::Vector2::Zero();
    Math::Vector2 position = parent_position + position_;
    Renderer::Get()->DrawBitmap(texture_->GetTexture(), position, size_, filter_mode_);
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
