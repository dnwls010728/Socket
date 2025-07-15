#include "pch.h"
#include "UIButton.h"

#include "UIImage.h"
#include "UIText.h"
#include "Windows/DX/UISprite.h"

UIButton::UIButton(const std::wstring& name) : 
    UIContainer(name),
    sprites_(),
    current_state_(State::kNormal)
{
    ui_image_ = AddChild<UIImage>(UIImage::StaticClass(), L"Image");
    
    ui_text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    ui_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    ui_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void UIButton::SetDisabled(bool is_disabled)
{
    ChangeState(is_disabled ? State::kDisabled : State::kNormal);
}

void UIButton::SetSprite(State state, UISprite* sprite, const std::wstring& frame_name)
{
    sprites_[static_cast<uint8_t>(state)] = std::make_pair(sprite, frame_name);
}

void UIButton::ChangeState(State state)
{
    current_state_ = state;

    std::pair<UISprite*, std::wstring> sprite = sprites_[static_cast<uint8_t>(state)];
    ui_image_->SetSprite(sprite.first, sprite.second);
}

void UIButton::Init()
{
    UIContainer::Init();

    ui_image_->SetSize(GetSize());
    ui_text_->SetSize(GetSize());
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIButton>("UIButton")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
