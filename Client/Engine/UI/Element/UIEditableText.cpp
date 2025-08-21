#include "pch.h"
#include "UIEditableText.h"

#include <cwctype>
#include <numeric>

#include "Scancode.h"
#include "UIMask.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIEditableText::UIEditableText(const std::wstring& name) :
    UIMask(name),
    cursor_position_(0),
    character_limit_(0),
    timer_(0.f),
    cursor_advance_(0.f),
    text_offset_(0.f),
    cursor_visible_(false),
    text_buffer_(L""),
    content_type_(ContentType::kStandard),
    value_changed_event_([](const std::wstring& value){}),
    return_event_([](){})
{
    text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    text_->SetColor(Math::Color::White);

    placeholder_text_ = AddChild<UIText>(UIText::StaticClass(), L"PlaceholderText");
    placeholder_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    placeholder_text_->SetColor(Math::Color::Gray);
}

void UIEditableText::SetSize(const Math::Vector2& size)
{
    UIMask::SetSize(size);
    
    text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });
    ScrollToCursor();
}

void UIEditableText::SetText(const std::wstring& text)
{
    cursor_position_ = text.size();
    text_buffer_ = text;
    UpdateDisplayedText();
    PostTextChange(false);
}

void UIEditableText::SetContentType(ContentType type)
{
    content_type_ = type;
    UpdateDisplayedText();
    PostTextChange(false);
}

void UIEditableText::Init()
{
    placeholder_text_->SetSize(GetSize());
    
    UIMask::Init();
}

void UIEditableText::Tick(float delta_time)
{
    UIMask::Tick(delta_time);

    if (IsFocused())
    {
        timer_ += delta_time;
        if (timer_ > .5f)
        {
            cursor_visible_ = !cursor_visible_;
            timer_ = 0.f;
        }
    }
}

void UIEditableText::Render()
{
    Renderer* renderer = Renderer::Get();
    UIMask::Render();

    if (cursor_visible_)
    {
        float font_height = text_->GetFontHeight();
        float margin = (GetSize().y - font_height) * .5f;
        
        Math::Vector2 position = GetAbsolutePosition();

        Math::Vector2 start = {position.x + cursor_advance_ - text_offset_, position.y + margin};
        Math::Vector2 end = {position.x + cursor_advance_ - text_offset_, position.y + GetSize().y - margin};

        renderer->DrawLine(start, end, Math::Color::White, 2.f);
    }
}

bool UIEditableText::OnKey(uint32_t scancode, bool is_pressed)
{
    if (!is_pressed) return false;

    switch (scancode)
    {
    case static_cast<uint32_t>(Scancode::kKeyLeft):
        {
            if (cursor_position_ > 0)
            {
                --cursor_position_;
                PostCursorMove();
            }
        }
        break;

    case static_cast<uint32_t>(Scancode::kKeyRight):
        {
            if (cursor_position_ < text_buffer_.size())
            {
                ++cursor_position_;
                PostCursorMove();
            }
        }
        break;

    case static_cast<uint32_t>(Scancode::kKeyEnter):
        {
            ResetCursor();
            return_event_();
        }
        break;

    case static_cast<uint32_t>(Scancode::kKeyBackspace):
        {
            if (!text_buffer_.empty() && cursor_position_ > 0)
            {
                text_buffer_.erase(--cursor_position_, 1);

                UpdateDisplayedText();
                PostTextChange(true);
            }
        }
        break;

    case static_cast<uint32_t>(Scancode::kKeyHome):
        {
            cursor_position_ = 0;
            PostCursorMove();
        }
        break;

    case static_cast<uint32_t>(Scancode::kKeyEnd):
        {
            cursor_position_ = text_buffer_.size();
            PostCursorMove();
        }
        break;

    case static_cast<uint32_t>(Scancode::kKeyDelete):
        {
            if (!text_buffer_.empty() && cursor_position_ < text_buffer_.size())
            {
                text_buffer_.erase(cursor_position_, 1);
                
                UpdateDisplayedText();
                PostTextChange(true);
            }
        }
        break;

    default:
        return false;
        
    }
    
    return true;
}

bool UIEditableText::OnChar(wchar_t character)
{
    if (character_limit_ > 0 && text_buffer_.size() >= character_limit_) return false;
    if (content_type_ == ContentType::kIntegerNumber)
    {
        if (!std::iswdigit(character) && character != L'-' && character != L'+') return false;
        if (text_buffer_.empty() && (character == L'-' || character == L'+')) return false;
    }
    else if (content_type_ == ContentType::kDecimalNumber)
    {
        if (!std::iswdigit(character) && character != L'.' && character != L'-' && character != L'+') return false;
        if (text_buffer_.empty() && (character == L'-' || character == L'+')) return false;
        if (character == L'.' && text_buffer_.find(L'.') != std::wstring::npos) return false;
    }
    else if (content_type_ == ContentType::kAlphanumeric)
        if (!std::iswalnum(character) && character != L' ') return false;
    
    text_buffer_.insert(cursor_position_++, 1, character);

    UpdateDisplayedText();
    PostTextChange(true);
    return true;
}

void UIEditableText::OnFocus(bool is_focused)
{
    cursor_visible_ = is_focused;
    timer_ = 0.f;
    
    UIMask::OnFocus(is_focused);
}

void UIEditableText::PostCursorMove()
{
    ScrollToCursor();
    ResetCursor();
}

void UIEditableText::UpdateDisplayedText() const
{
    if (content_type_ == ContentType::kPassword)
    {
        text_->SetText(std::wstring(text_buffer_.size(), L'*'));
        return;
    }
    
    text_->SetText(text_buffer_);
}

void UIEditableText::PostTextChange(bool is_reset)
{
    value_changed_event_(text_buffer_);
    text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });

    UpdatePlaceholder();
    ScrollToCursor();

    if (is_reset) ResetCursor();
}

void UIEditableText::UpdatePlaceholder() const
{
    const bool is_empty = text_buffer_.empty();
    placeholder_text_->SetActive(is_empty);
}

void UIEditableText::ResetCursor()
{
    cursor_visible_ = true;
    timer_ = 0.f;
}

void UIEditableText::ScrollToCursor()
{
    const auto& advances = text_->GetAdvances();
    
    cursor_position_ = Math::Clamp(cursor_position_, 0.f, advances.size());

    cursor_advance_ = 0.f;
    if (!advances.empty() && cursor_position_ > 0)
        cursor_advance_ = std::accumulate(advances.begin(), advances.begin() + cursor_position_, 0.f);

    const float view_width = GetSize().x;
    const float text_width = text_->GetTotalAdvance();

    if (text_width <= view_width)
        text_offset_ = 0.f;
    else
    {
        if (cursor_advance_ < text_offset_)
            text_offset_ = cursor_advance_;
        else if (cursor_advance_ > text_offset_ + view_width)
            text_offset_ = cursor_advance_ - view_width;
        
        text_offset_ = Math::Clamp(text_offset_, 0.f, text_width - view_width);
    }

    text_->SetAbsolutePosition({GetAbsolutePosition().x - text_offset_, GetAbsolutePosition().y});
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIEditableText>("UIEditableText")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
