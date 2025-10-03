#include "pch.h"
#include "UISkillTooltip.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/UISprite.h"

UISkillTooltip::UISkillTooltip(const std::wstring& name)
    : UIContainer(name),
      background_(nullptr),
      name_text_(nullptr),
      level_text_(nullptr),
      desc_text_(nullptr)
{
    SetIgnoreRayCast(true);
    SetSize({ 200.f, 100.f });
    SetActive(false);
}

void UISkillTooltip::Init()
{
    UIContainer::Init();

    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetSize(GetSize());
    background_->SetIgnoreRayCast(true);

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetRelativePosition({ 8.f, 6.f });
    name_text_->SetSize({ 184.f, 20.f });
    name_text_->SetColor(Math::Color::White);
    name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    name_text_->SetIgnoreRayCast(true);

    level_text_ = AddChild<UIText>(UIText::StaticClass(), L"LevelText");
    level_text_->SetRelativePosition({ 8.f, 28.f });
    level_text_->SetSize({ 184.f, 16.f });
    level_text_->SetColor(Math::Color::White);
    level_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    level_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    level_text_->SetIgnoreRayCast(true);

    desc_text_ = AddChild<UIText>(UIText::StaticClass(), L"DescText");
    desc_text_->SetRelativePosition({ 8.f, 48.f });
    desc_text_->SetSize({ 184.f, 44.f });
    desc_text_->SetColor(Math::Color::White);
    desc_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    desc_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    desc_text_->SetIgnoreRayCast(true);
}

void UISkillTooltip::SetSkillInfo(const std::wstring& name, int32_t level, const std::wstring& description)
{
    if (name_text_)  name_text_->SetText(name);
    if (level_text_) level_text_->SetText(L"Lv. " + std::to_wstring(level));
    if (desc_text_)  desc_text_->SetText(description);
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UISkillTooltip>("UISkillTooltip")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}