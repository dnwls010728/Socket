#include "pch.h"
#include "UIQuickSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UIQuickSlot::UIQuickSlot(const std::wstring& name) :
    UIContainer(name)
{
    SetSize({ 32.f, 32.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSize(GetSize());
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetSize(GetSize());
    icon_->SetIgnoreRayCast(true);

    key_name_text_ = AddChild<UIText>(UIText::StaticClass(), L"KeyNameText");
    key_name_text_->SetSize(GetSize());
    key_name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    key_name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    key_name_text_->SetColor(Math::Color::White);
    key_name_text_->SetText(L"Shift");
    key_name_text_->SetIgnoreRayCast(true);

    count_text_ = AddChild<UIText>(UIText::StaticClass(), L"CountText");
    count_text_->SetSize(GetSize());
    count_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    count_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    count_text_->SetColor(Math::Color::White);
    count_text_->SetText(L"99");
    count_text_->SetIgnoreRayCast(true);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIQuickSlot>("UIQuickSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
