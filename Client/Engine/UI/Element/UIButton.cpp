#include "pch.h"
#include "UIButton.h"

#include "UIImage.h"
#include "UIText.h"
#include "Windows/DX/UISprite.h"

UIButton::UIButton(const std::wstring& name) : 
    UIContainer(name),
    sprites_(),
    current_state_(State::kNormal),
    click_event_([]() {})
{
    ui_image_ = AddChild<UIImage>(UIImage::StaticClass(), L"Image");
    ui_image_->SetIgnoreRayCast(true);
    
    ui_text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    ui_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    ui_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    ui_text_->SetIgnoreRayCast(true);
}

void UIButton::SetDisabled(bool is_disabled)
{
    ChangeState(is_disabled ? State::kDisabled : State::kNormal);
}

void UIButton::SetSprite(State state, UISprite* sprite, const std::wstring& frame_name)
{
    sprites_[static_cast<uint8_t>(state)] = std::make_pair(sprite, frame_name);
    if (current_state_ == state)
    {
        ui_image_->SetSprite(sprite, frame_name);
    }
}

void UIButton::SetDrawMode(UIImage::DrawMode draw_mode) const
{
    ui_image_->SetDrawMode(draw_mode);
}

void UIButton::SetText(const std::wstring& text) const
{
    ui_text_->SetText(text);
}

void UIButton::SetTextColor(const Math::Color& color) const
{
    ui_text_->SetColor(color);
}

void UIButton::ChangeState(State state)
{
    current_state_ = state;

    std::pair<UISprite*, std::wstring> sprite = sprites_[static_cast<uint8_t>(current_state_)];
    ui_image_->SetSprite(sprite.first, sprite.second);
}

void UIButton::Init()
{
    UIContainer::Init();

    ui_image_->SetSize(GetSize());
    ui_text_->SetSize(GetSize());

    std::pair<UISprite*, std::wstring> sprite = sprites_[static_cast<uint8_t>(current_state_)];
    ui_image_->SetSprite(sprite.first, sprite.second);
}

bool UIButton::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    bool result = UIContainer::OnMouseButton(position, button, is_pressed, timestamp);
    if (current_state_ == State::kDisabled) return result;

    if (button == MouseButton::kLeft && is_pressed)
    {
        ChangeState(State::kPressed);
        result = true;
        
        click_event_();
    }
    else if (button == MouseButton::kLeft && !is_pressed)
    {
        ChangeState(State::kNormal);
        result = true;
    }

    return result;
}

bool UIButton::OnMouseEnter()
{
    if (current_state_ == State::kDisabled) return false;
    
    ChangeState(State::kHover);
    return true;
}

bool UIButton::OnMouseLeave()
{
    if (current_state_ == State::kDisabled) return false;
    
    ChangeState(State::kNormal);
    return true;
}

void UIButton::SetSize(const Math::Vector2& size)
{
    UIContainer::SetSize(size);
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
