#include "pch.h"
#include "UIImage.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIImage::UIImage(const std::wstring& name) :
    UIElement(name),
    ui_sprite_(nullptr),
    current_frame_(L""),
    draw_mode_(DrawMode::kSimple),
    color_(Math::Color::White)
{
}

void UIImage::SetSprite(UISprite* ui_sprite, const std::wstring& frame_name)
{
    ui_sprite_ = ui_sprite;
    current_frame_ = frame_name;
}

void UIImage::Render()
{
    UIElement::Render();
    if (!ui_sprite_) return;
    
    if (draw_mode_ == DrawMode::kSimple) Renderer::Get()->DrawSimpleSprite(ui_sprite_, current_frame_, GetAbsolutePosition(), GetSize(), color_);
    else if (draw_mode_ == DrawMode::kSliced) Renderer::Get()->DrawSlicedSprite(ui_sprite_, current_frame_, GetAbsolutePosition(), GetSize(), color_);
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
