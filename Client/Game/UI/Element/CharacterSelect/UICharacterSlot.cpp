#include "pch.h"
#include "UICharacterSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UICharacterSlot::UICharacterSlot(const std::wstring& name) :
    UIContainer(name)
{
    size_ = { 200.f,  228.f };
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    empty_text_ = AddChild<UIText>(UIText::StaticClass(), L"EmptyText");
    empty_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    empty_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    empty_text_->SetColor(Math::Color::White);
    empty_text_->SetText(L"EMPTY");
}

void UICharacterSlot::InitSlot(const CharacterProfile& profile)
{
    if (profile.character_id == 0) return;
    empty_text_->SetActive(false);
}

void UICharacterSlot::Init()
{
    background_->SetSize(GetSize());
    empty_text_->SetSize(GetSize());
    
    UIContainer::Init();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UICharacterSlot>("UICharacterSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
