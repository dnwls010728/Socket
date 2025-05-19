#include "pch.h"
#include "UIText.h"

#include "Windows/DX/Renderer.h"

UIText::UIText() :
    text_(L"New Text"),
    font_name_(L"NanumBarunGothic"),
    font_size_(12.f),
    color_(Math::Color::Black),
    text_alignment_(DWRITE_TEXT_ALIGNMENT_LEADING),
    paragraph_alignment_(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
{
}

void UIText::Render(const Math::Vector2& parent_position)
{
    UIElement::Render(parent_position);

    Math::Vector2 position = parent_position + position_;
    Renderer::Get()->DrawString(text_, position, size_, color_, font_name_, font_size_, text_alignment_, paragraph_alignment_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIText>("UIText")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
