#include "pch.h"
#include "UIChatBalloon.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIChatBalloon::UIChatBalloon(const std::wstring& name) :
    UIContainer(name)
{
    UISprite* sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ChatBalloon.png");
    
    body_ = AddChild<UIImage>(UIImage::StaticClass(), L"Body");
    body_->SetSprite(sprite, L"ChatBalloon_0");
    body_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    tail_ = AddChild<UIImage>(UIImage::StaticClass(), L"Tail");
    tail_->SetSize({ 32.f, 32.f });
    tail_->SetSprite(sprite, L"ChatBalloon_1");

    text_ = AddChild<UIText>(UIText::StaticClass(), L"Text");
    text_->SetRelativePosition({ 16.f, 16.f });
    text_->SetSize({ 128.f, 32.f });
    text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    
    SetIgnoreRayCast(true);
    
}

void UIChatBalloon::SetText(const std::wstring& text)
{
    text_->SetText(text);

    Math::Vector2 text_size = text_->GetSize();
    float total_line_height = text_->GetTotalLineHeight();
    text_->SetSize({ text_size.x, total_line_height });

    body_->SetSize({ text_->GetSize().x + 32.f, text_->GetSize().y + 32.f });

    float tail_offset_x = body_->GetSize().x * .5f - tail_->GetSize().x * .5f;
    float tail_offset_y = body_->GetSize().y - tail_->GetSize().y * .5f - 3.f;
    tail_->SetRelativePosition({ tail_offset_x, tail_offset_y });

    SetSize({body_->GetSize().x, body_->GetSize().y + tail_->GetSize().y});
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIChatBalloon>("UIChatBalloon")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
