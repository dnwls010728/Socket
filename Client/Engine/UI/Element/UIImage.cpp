#include "pch.h"
#include "UIImage.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIImage::UIImage(const std::wstring& name) :
    UIElement(name),
    ui_sprite_(nullptr),
    current_frame_(L"")
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
    
    Renderer::Get()->DrawSprite(ui_sprite_, current_frame_, GetAbsolutePosition(), size_);
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
