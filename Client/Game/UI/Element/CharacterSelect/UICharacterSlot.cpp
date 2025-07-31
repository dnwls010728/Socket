#include "pch.h"
#include "UICharacterSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UICharacterSlot::UICharacterSlot(const std::wstring& name) :
    UIContainer(name),
    character_id_(0)
{
    size_ = { 200.f,  228.f };
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    character_ = AddChild<UIImage>(UIImage::StaticClass(), L"Character");
    character_->SetRelativePosition({ 10.f, 10.f });
    character_->SetSize({ 180.f, 140.f });
    character_->SetActive(false);
    character_->SetIgnoreRayCast(true);

    empty_text_ = AddChild<UIText>(UIText::StaticClass(), L"EmptyText");
    empty_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    empty_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    empty_text_->SetColor(Math::Color::White);
    empty_text_->SetText(L"EMPTY");
    empty_text_->SetIgnoreRayCast(true);

    lv_text_ = AddChild<UIText>(UIText::StaticClass(), L"LvText");
    lv_text_->SetRelativePosition({ 10.f, 160.f });
    lv_text_->SetSize({ 180.f, 20.f });
    lv_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    lv_text_->SetColor(Math::Color::White);
    lv_text_->SetActive(false);
    lv_text_->SetIgnoreRayCast(true);

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetRelativePosition({ 10.f, 180.f });
    name_text_->SetSize({ 180.f, 20.f });
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    name_text_->SetColor(Math::Color::White);
    name_text_->SetActive(false);
    name_text_->SetIgnoreRayCast(true);

    color_code_text_ = AddChild<UIText>(UIText::StaticClass(), L"ColorCodeText");
    color_code_text_->SetRelativePosition({ 10.f, 200.f });
    color_code_text_->SetSize({ 180.f, 20.f });
    color_code_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    color_code_text_->SetColor(Math::Color::White);
    color_code_text_->SetActive(false);
    color_code_text_->SetIgnoreRayCast(true);
}

void UICharacterSlot::InitSlot(const CharacterProfile& profile)
{
    if (profile.character_id == 0) return;
    character_id_ = profile.character_id;

    character_->SetActive(true);
    empty_text_->SetActive(false);
    lv_text_->SetActive(true);
    name_text_->SetActive(true);
    color_code_text_->SetActive(true);

    UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");
    character_->SetSprite(character_sprite, L"UIPlayerSheet_0");
    character_->SetColor(Math::Color::HexToColor(profile.character_color));

    int32_t lv = profile.stats[static_cast<uint8_t>(PlayerStat::kLv)];
    lv_text_->SetText(L"레벨 " + std::to_wstring(lv));
    
    name_text_->SetText(profile.name);
    color_code_text_->SetText(L"색상 코드: #" + profile.character_color);
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
