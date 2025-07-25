#include "pch.h"
#include "UINameTag.h"

#include "Windows/DX/Renderer.h"

UINameTag::UINameTag(const std::wstring& name) :
    UIContainer(name)
{
    text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    text_->SetColor(Math::Color::White);
}

void UINameTag::SetText(const std::wstring& text)
{
    text_->SetText(text);
    text_->SetSize({ text_->GetTotalAdvance() + 16.f, 20.f });

    SetSize(text_->GetSize());
}

void UINameTag::SetColor(const Math::Color& color) const
{
    text_->SetColor(color);
}

void UINameTag::Init()
{
    UIContainer::Init();
}

void UINameTag::Render()
{
    Renderer::Get()->DrawSolidRoundBox(GetAbsolutePosition(), size_, {0, 0, 0, 128});
    
    UIContainer::Render();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UINameTag>("UINameTag")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
