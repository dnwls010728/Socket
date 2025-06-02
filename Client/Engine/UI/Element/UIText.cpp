#include "pch.h"
#include "UIText.h"

#include "UI/UIContainer.h"
#include "Windows/DX/Renderer.h"

UIText::UIText(const std::wstring& name) :
    UIElement(name),
    text_(L""),
    font_name_(L"NanumBarunGothic"),
    font_size_(12.f),
    color_(Math::Color::Black),
    text_alignment_(DWRITE_TEXT_ALIGNMENT_LEADING),
    paragraph_alignment_(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
{
}

void UIText::Render()
{
    UIElement::Render();

    Renderer::Get()->DrawString(text_, GetAbsolutePosition(), size_, color_, font_name_, font_size_, text_alignment_, paragraph_alignment_);
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
