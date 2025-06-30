#include "pch.h"
#include "UIImage.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIImage::UIImage(const std::wstring& name) :
    UIElement(name),
    ui_sprite_(nullptr),
    timer_(0.f),
    frame_index_(0)
{
}

void UIImage::Tick(float delta_time)
{
    UIElement::Tick(delta_time);

    timer_ += delta_time;
    if (timer_ >= 1.f / 2.f)
    {
        timer_ -= 1.f / 2.f;
        if (ui_sprite_)
        {
            frame_index_ = (frame_index_ + 1) % 4;
        }
    }
}

void UIImage::Render()
{
    UIElement::Render();
    if (!ui_sprite_) return;

    // Renderer::Get()->DrawBitmap(ui_sprite_->GetTexture(), GetAbsolutePosition(), size_, filter_mode_);
    Renderer::Get()->DrawSprite(ui_sprite_, L"LoginBackground_" + std::to_wstring(frame_index_), GetAbsolutePosition(), size_);
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
