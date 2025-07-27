#include "pch.h"
#include "UIText.h"

#include <numeric>

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"

UIText::UIText(const std::wstring& name) :
    UIElement(name),
    text_(L""),
    font_name_(L"NanumBarunGothic"),
    font_size_(12.f),
    color_(Math::Color::Black),
    text_alignment_(DWRITE_TEXT_ALIGNMENT_LEADING),
    paragraph_alignment_(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
    advances_(),
    total_advance_(0.f)
{
}

void UIText::SetText(const std::wstring& text)
{
    text_ = text;
    UpdateAdvances();
}

void UIText::SetFontName(const std::wstring& font_name)
{
    font_name_ = font_name;
    UpdateAdvances();
}

void UIText::SetFontSize(float font_size)
{
    font_size_ = font_size;
    UpdateAdvances();
}

float UIText::GetFontHeight() const
{
    return Renderer::Get()->GetTextFormat(font_name_, font_size_)->GetFontSize();
}

void UIText::Render()
{
    UIElement::Render();

    Renderer::Get()->DrawString(text_, GetAbsolutePosition(), size_, color_, font_name_, font_size_, text_alignment_, paragraph_alignment_);
    // Renderer::Get()->DrawStringWithOutline(text_, GetAbsolutePosition(), size_, Math::Color::Black, color_, 1.f, font_name_, font_size_, text_alignment_, paragraph_alignment_);
}

void UIText::UpdateAdvances()
{
    Renderer* renderer = Renderer::Get();
    renderer->GetTextAdvances(text_, font_name_, font_size_, advances_);

    total_advance_ = std::accumulate(advances_.begin(), advances_.end(), 0.f);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIText>("UIText")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
