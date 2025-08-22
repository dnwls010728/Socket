#include "pch.h"
#include "UIImage.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIImage::UIImage(const std::wstring& name) :
    UIElement(name),
    ui_sprite_(nullptr),
    frame_index_(0),
    draw_mode_(DrawMode::kSimple),
    color_(Math::Color::White)
{
}

void UIImage::SetSprite(UISprite* ui_sprite, const std::wstring& frame_name)
{
    ui_sprite_ = ui_sprite;
    if (!ui_sprite_) return;

    const auto& frame_indexes = ui_sprite_->GetFrameIndexes();
    
    auto it = frame_indexes.find(frame_name);
    if (it != frame_indexes.end())
        frame_index_ = it->second;
    else frame_index_ = 0;
}

void UIImage::SetSprite(UISprite* ui_sprite, uint64_t frame_index)
{
    ui_sprite_ = ui_sprite;
    frame_index_ = frame_index;
}

void UIImage::Render()
{
    UIElement::Render();
    if (!ui_sprite_) return;
    
    if (draw_mode_ == DrawMode::kSimple) Renderer::Get()->DrawSimpleSprite(ui_sprite_, frame_index_, GetAbsolutePosition(), GetSize(), color_);
    else if (draw_mode_ == DrawMode::kSliced) Renderer::Get()->DrawSlicedSprite(ui_sprite_, frame_index_, GetAbsolutePosition(), GetSize(), color_);
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
