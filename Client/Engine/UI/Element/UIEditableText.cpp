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
    cursor_visible_(false)
{
    text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    text_->SetColor(Math::Color::White);
}

void UIEditableText::SetText(const std::wstring& text)
{
    text_->SetText(text);
    cursor_position_ = text.size();
    text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });

    ScrollToCursor();
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
        Math::Vector2 position = GetAbsolutePosition();

        Math::Vector2 start = {position.x + cursor_advance_ - text_offset_, position.y};
        Math::Vector2 end = {position.x + cursor_advance_ - text_offset_, position.y + size_.y};

        renderer->DrawLine(start, end, Math::Color::White, 2.f);
    }
}

UI::MouseEventResult UIEditableText::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    return UIMask::OnMouseButton(position, button, is_pressed, timestamp);
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
                ScrollToCursor();
                
                cursor_visible_ = true;
                timer_ = 0.f;
            }
        }
        break;

    case VK_RIGHT:
        {
            if (cursor_position_ < text_->GetText().size())
            {
                ++cursor_position_;
                ScrollToCursor();
                
                cursor_visible_ = true;
                timer_ = 0.f;
            }
        }
        break;

    case VK_BACK:
        {
            if (!text_->GetText().empty() && cursor_position_ > 0)
            {
                std::wstring current_text = text_->GetText();
                current_text.erase(--cursor_position_, 1);
                
                text_->SetText(current_text);
                text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });

                ScrollToCursor();
                
                cursor_visible_ = true;
                timer_ = 0.f;
            }
        }
        break;

    case VK_HOME:
        {
            cursor_position_ = 0;
            ScrollToCursor();
            
            cursor_visible_ = true;
            timer_ = 0.f;
        }
        break;

    case VK_END:
        {
            cursor_position_ = text_->GetText().size();
            ScrollToCursor();
            
            cursor_visible_ = true;
            timer_ = 0.f;
        }
        break;

    case VK_DELETE:
        {
            if (!text_->GetText().empty() && cursor_position_ < text_->GetText().size())
            {
                std::wstring current_text = text_->GetText();
                current_text.erase(cursor_position_, 1);
                
                text_->SetText(current_text);
                text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });

                ScrollToCursor();
                
                cursor_visible_ = true;
                timer_ = 0.f;
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

    if (character == 32)
    {
        cursor_visible_ = true;
        timer_ = 0.f;
    }
    
    text_->SetText(current_text);
    text_->SetSize({ text_->GetTotalAdvance() + 1.f, GetSize().y });

    ScrollToCursor();
    
    return true;
}

void UIEditableText::OnFocus(bool is_focused)
{
    cursor_visible_ = is_focused;
    timer_ = 0.f;
    
    UIMask::OnFocus(is_focused);
}

void UIEditableText::ScrollToCursor() {
    const auto& advances = text_->GetAdvances();

    // 안전한 인덱스
    const size_t cur = std::min<size_t>(cursor_position_, advances.size());

    // 커서 x 위치(텍스트 시작 기준 누적 advance)
    float cursor_x = std::accumulate(advances.begin(), advances.begin() + cur, 0.f);

    const float view_w = GetSize().x;
    const float text_w = std::accumulate(advances.begin(), advances.end(), 0.f);

    // 텍스트 전체가 뷰 안이면 스크롤 불필요
    if (text_w <= view_w) {
        text_offset_ = 0.f;
    } else {
        // 커서가 왼쪽 밖이면 왼쪽으로 당김
        if (cursor_x < text_offset_) {
            text_offset_ = cursor_x;
        }
        // 커서가 오른쪽 밖이면 오른쪽으로 밀어줌
        else if (cursor_x > text_offset_ + view_w) {
            text_offset_ = cursor_x - view_w;
        }

        // 오프셋 클램프
        text_offset_ = Math::Clamp(text_offset_, 0.f, text_w - view_w);
    }

    // 실제 위치 갱신은 매번 수행
    text_->SetAbsolutePosition({
        GetAbsolutePosition().x - text_offset_,
        GetAbsolutePosition().y
    });

    // (선택) 디버그용으로 저장
    cursor_advance_ = cursor_x;
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
