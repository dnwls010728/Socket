#include "pch.h"
#include "UISkillSlot.h"

#include "UISkillTooltip.h"
#include "Asset/AssetManager.h"
#include "Subsystems/DataSubsystem.h"
#include "UI/Element/Skill/UISkillWindow.h"
#include "UI/Element/UIImage.h"
#include "Windows/DX/UISprite.h"

UISkillSlot::UISkillSlot(const std::wstring& name) :
    UIContainer(name),
    owner_(nullptr),
    background_(nullptr),
    icon_(nullptr),
    name_text_(nullptr),
    level_text_(nullptr),
    skill_id_(0),
    level_(0),
    cooldown_(0.f)
{
    SetSize({ 180.f, 40.f });

    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetRelativePosition({ 4.f, 4.f });
    icon_->SetSize({ 32.f, 32.f });
    icon_->SetSprite(nullptr, L"");
    icon_->SetDrawMode(UIImage::DrawMode::kSimple);
    icon_->SetColor(Math::Color::White);
    icon_->SetIgnoreRayCast(true);

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetRelativePosition({ 44.f, 6.f });
    name_text_->SetSize({ 132.f, 18.f });
    name_text_->SetColor(Math::Color::White);
    name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    name_text_->SetIgnoreRayCast(true);

    level_text_ = AddChild<UIText>(UIText::StaticClass(), L"LevelText");
    level_text_->SetRelativePosition({ 44.f, 22.f });
    level_text_->SetSize({ 132.f, 14.f });
    level_text_->SetColor(Math::Color::White);
    level_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    level_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    level_text_->SetIgnoreRayCast(true);
}

void UISkillSlot::SetSkill(const SkillInfo& skill_info)
{
    skill_id_ = skill_info.skill_id;
    level_ = skill_info.level;
    cooldown_ = skill_info.cooldown;

    auto set_empty_icon_lambda = [this](){
        icon_->SetSprite(nullptr, L"");
        icon_->SetActive(false);
    };

    const SkillData* data = DataSubsystem::Get()->GetSkill(skill_id_);
    if (!data)
    {
        name_text_->SetText(L"알 수 없는 스킬");
        set_empty_icon_lambda();
        return;
    }

    name_text_->SetText(data->name);
    level_text_->SetText(L"Lv. " + std::to_wstring(level_));
    description_ = data->desc;

    const SkillData* sprite_data = data;
    if (data->type == SkillType::kComboAttack)
    {
        if (data->combo_skills.empty() || data->combo_skills.size() < skill_info.combo_idx + 1)
        {
            set_empty_icon_lambda();
            SetActive(true);
            return;
        }

        uint32_t sub_skill_id = data->combo_skills[skill_info.combo_idx];
        sprite_data = DataSubsystem::Get()->GetSkill(sub_skill_id);
        if (!sprite_data)
        {
            set_empty_icon_lambda();
            SetActive(true);
            return;
        }
    }
    
    static UISprite* skill_icon_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\SkillIconSet.png");
    if (skill_icon_sprite && !sprite_data->icon.empty())
    {
        icon_->SetSprite(skill_icon_sprite, sprite_data->icon);
        icon_->SetActive(true);
    }
    else
    {
        set_empty_icon_lambda();
    }
    
    SetActive(true);
}

void UISkillSlot::Reset()
{
    skill_id_ = 0;
    level_ = 0;
    cooldown_ = 0.f;

    name_text_->SetText(L"");
    level_text_->SetText(L"");
    icon_->SetSprite(nullptr, L"");
    icon_->SetActive(false);
    SetActive(false);
}

void UISkillSlot::Init()
{
    background_->SetSize(GetSize());
    UIContainer::Init();
}

bool UISkillSlot::OnDragBegin(const Math::Vector2& position)
{
    if (!owner_ || skill_id_ == 0) return false;

    Math::Vector2 icon_position = icon_->GetAbsolutePosition();
    Math::Vector2 icon_size = icon_->GetSize();
    Math::Rect icon_rect(icon_position.x, icon_position.y, icon_size.x, icon_size.y );

    if (!Math::Rect::Contains(icon_rect, position))
        return false;

    UIImage* dragging_icon = owner_->GetDraggingIcon();
    if (!dragging_icon) return false;

    dragging_icon->SetSprite(icon_->GetSprite(), icon_->GetFrameIndex());
    dragging_icon->SetDrawMode(icon_->GetDrawMode());
    dragging_icon->SetColor(icon_->GetColor());
    dragging_icon->SetSize(icon_->GetSize());
    dragging_icon->SetActive(true);
    dragging_icon->SetAbsolutePosition(position - dragging_icon->GetSize() * .5f);

    return true;
}

bool UISkillSlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (!owner_ || skill_id_ == 0) return false;

    UIImage* dragging_icon = owner_->GetDraggingIcon();
    if (!dragging_icon || !dragging_icon->IsActive()) return false;

    dragging_icon->SetAbsolutePosition(position - dragging_icon->GetSize() * .5f);

    UISkillTooltip* tooltip = owner_->GetTooltip();
    if (tooltip)
    {
        tooltip->SetActive(false);
    }
    return true;
}


bool UISkillSlot::OnDragEnd(const Math::Vector2& position)
{
    if (!owner_) return false;

    if (UIImage* dragging_icon = owner_->GetDraggingIcon())
    {
        dragging_icon->SetActive(false);
        dragging_icon->SetSprite(nullptr, L"");
    }

    return UIContainer::OnDragEnd(position);
}

bool UISkillSlot::OnMouseEnter()
{
    if (!owner_ || skill_id_ == 0)
        return false;

    UISkillTooltip* tooltip = owner_->GetTooltip();
    if (!tooltip)
        return false;

    tooltip->SetSkillInfo(skill_id_, level_);
    return true;
}

bool UISkillSlot::OnMouseMotion(const Math::Vector2& position, const Math::Vector2& delta)
{
    if (!owner_ || skill_id_ == 0)
        return false;

    UISkillTooltip* tooltip = owner_->GetTooltip();
    if (!tooltip)
        return false;

    tooltip->SetActive(true);
    
    EngineSettings* settings = EngineSettings::Get();
    Math::Vector2 tooltip_size = tooltip->GetSize();
    Math::Vector2 tooltip_position = position + Math::Vector2::Up() * 32.f;

    int32_t screen_width = settings->GetScreenWidth();
    int32_t screen_height = settings->GetScreenHeight();

    int32_t overflow_width = tooltip_position.x + tooltip_size.x - screen_width;
    int32_t overflow_height = tooltip_position.y + tooltip_size.y - screen_height;

    if (overflow_width > 0) tooltip_position.x -= overflow_width;
    if (overflow_height > 0) tooltip_position.y -= overflow_height;

    tooltip->SetAbsolutePosition(tooltip_position);
    return true;
}

bool UISkillSlot::OnMouseLeave()
{
    if (!owner_ || skill_id_ == 0)
        return false;

    UISkillTooltip* tooltip = owner_->GetTooltip();
    if (!tooltip)
        return false;

    tooltip->SetActive(false);
    return true;
}


RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UISkillSlot>("UISkillSlot")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}