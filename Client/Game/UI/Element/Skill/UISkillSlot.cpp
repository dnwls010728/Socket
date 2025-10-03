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
    icon_->SetSprite(panel_sprite, L"Panel_0");
    icon_->SetDrawMode(UIImage::DrawMode::kSliced);
    icon_->SetColor({ 80, 120, 200, 255 });
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

void UISkillSlot::SetSkill(uint32_t skill_id, int32_t level, float cooldown)
{
    skill_id_ = skill_id;
    level_ = level;
    cooldown_ = cooldown;

    const SkillData* data = DataSubsystem::Get()->GetSkill(skill_id_);
    if (data)
    {
        name_text_->SetText(data->name);
        description_ = data->desc;
    }
    else
        name_text_->SetText(L"알 수 없는 스킬");

    level_text_->SetText(L"Lv. " + std::to_wstring(level_));
    SetActive(true);
}

void UISkillSlot::Reset()
{
    skill_id_ = 0;
    level_ = 0;
    cooldown_ = 0.f;

    name_text_->SetText(L"");
    level_text_->SetText(L"");
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
    if (!owner_) return true;

    UISkillTooltip* tooltip = owner_->GetTooltip();
    if (!tooltip) return true;

    Math::Vector2 position = GetAbsolutePosition();
    position.y += GetSize().y;
    position.x += GetSize().x;
    
    tooltip->SetAbsolutePosition(position);
    tooltip->SetSkillInfo(name_text_->GetText(), level_, description_);
    tooltip->SetActive(true);
    
    return true;
}

bool UISkillSlot::OnMouseLeave()
{
    if (!owner_) return true;

    UISkillTooltip* tooltip = owner_->GetTooltip();
    if (!tooltip) return true;

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