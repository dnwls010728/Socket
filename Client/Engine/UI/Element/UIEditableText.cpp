#include "pch.h"
#include "UIEditableText.h"

#include <numeric>

#include "UIMask.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIEditableText::UIEditableText(const std::wstring& name) :
    UIMask(name),
    cursor_position_(0),
    timer_(0.f),
    cursor_advance_(0.f),
    text_offset_(0.f),
    cursor_visible_(false),
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

void UIEditableText::SetText(const std::wstring& text)
{
    cursor_position_ = text.size();
    text_->SetText(text);
    PostTextChange(false);
}

void UIEditableText::OnValueChanged(void(* func)(const std::wstring&))
{
    value_changed_event_ = func;
}

void UIEditableText::OnReturn(void(* func)())
{
    return_event_ = func;
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
        float margin = (size_.y - font_height) * .5f;
        
        Math::Vector2 position = GetAbsolutePosition();

        Math::Vector2 start = {position.x + cursor_advance_ - text_offset_, position.y + margin};
        Math::Vector2 end = {position.x + cursor_advance_ - text_offset_, position.y + size_.y - margin};

        renderer->DrawLine(start, end, Math::Color::White, 2.f);
    }
}

bool UIEditableText::OnKey(uint16_t key_code, bool is_pressed)
{
    if (!is_pressed) return false;

    switch (key_code)
    {
    case VK_LEFT:
        {
            if (cursor_position_ > 0)
            {
                --cursor_position_;
                PostCursorMove();
            }
        }
        break;

    case VK_RIGHT:
        {
            if (cursor_position_ < text_->GetText().size())
            {
                ++cursor_position_;
                PostCursorMove();
            }
        }
        break;

    case VK_RETURN:
        {
            ResetCursor();
            return_event_();
        }
        break;

    case VK_BACK:
        {
            if (!text_->GetText().empty() && cursor_position_ > 0)
            {
                std::wstring current_text = text_->GetText();
                current_text.erase(--cursor_position_, 1);
                
                text_->SetText(current_text);
                PostTextChange(true);
            }
        }
        break;

    case VK_HOME:
        {
            cursor_position_ = 0;
            PostCursorMove();
        }
        break;

    case VK_END:
        {
            cursor_position_ = text_->GetText().size();
            PostCursorMove();
        }
        break;

    case VK_DELETE:
        {
            if (!text_->GetText().empty() && cursor_position_ < text_->GetText().size())
            {
                std::wstring current_text = text_->GetText();
                current_text.erase(cursor_position_, 1);
                
                text_->SetText(current_text);
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
    std::wstring current_text = text_->GetText();
    current_text.insert(cursor_position_++, 1, character);
    
    text_->SetText(current_text);
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

void UIEditableText::PostTextChange(bool is_reset)
{
    value_changed_event_(text_->GetText());
    text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });

    UpdatePlaceholder();
    ScrollToCursor();

    if (is_reset) ResetCursor();
}

void UIEditableText::UpdatePlaceholder() const
{
    const bool is_empty = text_->GetText().empty();
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
