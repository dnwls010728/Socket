#include "pch.h"
#include "UISkillTooltip.h"
#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "Windows/DX/UISprite.h"

UISkillTooltip::UISkillTooltip(const std::wstring& name)
    : UIContainer(name),
      background_(nullptr),
      name_text_(nullptr),
      level_text_(nullptr),
      desc_text_(nullptr)
{
    SetIgnoreRayCast(true);
    SetSize({ 322.f, 122.f });
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
    
    icon_slot_ = AddChild<UIImage>(UIImage::StaticClass(), L"IconSlot");
    icon_slot_->SetRelativePosition({ 10.f, 30.f });
    icon_slot_->SetSize({ 82.f, 82.f });
    icon_slot_->SetSprite(panel_sprite, L"Panel_0");
    icon_slot_->SetDrawMode(UIImage::DrawMode::kSliced);
    
    skill_icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"SkillIcon");
    skill_icon_->SetRelativePosition({ 20.f, 40.f });
    skill_icon_->SetSize({ 62.f, 62.f });

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetRelativePosition({ 10.f, 0.f });
    name_text_->SetSize({ 100.f, 30.f });
    name_text_->SetColor(Math::Color::White);
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    name_text_->SetIgnoreRayCast(true);

    level_text_ = AddChild<UIText>(UIText::StaticClass(), L"LevelText");
    level_text_->SetRelativePosition({ 102.f, 30.f });
    level_text_->SetSize({ 100.f, 30.f });
    level_text_->SetColor(Math::Color::White);
    level_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    level_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    level_text_->SetIgnoreRayCast(true);

    desc_text_ = AddChild<UIText>(UIText::StaticClass(), L"DescText");
    desc_text_->SetRelativePosition({ 102.f, 60.f });
    desc_text_->SetSize({ 210.f, 62.f });
    desc_text_->SetColor(Math::Color::White);
    desc_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    desc_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    desc_text_->SetIgnoreRayCast(true);
}

void UISkillTooltip::SetSkillInfo(uint32_t skill_id, int32_t level)
{
    const SkillData* skill_data = DataSubsystem::Get()->GetSkill(skill_id);
    if (!skill_data) return;

    UISprite* ui_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\SkillIconSet.png");
    if (!ui_sprite)
    {
        static UISprite* kMissing = AssetManager::Get()->Load<UISprite>(L"UI\\Item\\Missing.png");
        ui_sprite = kMissing;
    }
    
    if (skill_icon_) skill_icon_->SetSprite(ui_sprite, skill_data->icon);
    if (name_text_)  name_text_->SetText(skill_data->name);
    if (level_text_) level_text_->SetText(L"Lv. " + std::to_wstring(level));
    if (desc_text_)  desc_text_->SetText(skill_data->desc);
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