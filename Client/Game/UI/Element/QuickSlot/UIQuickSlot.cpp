#include "pch.h"
#include "UIQuickSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/Inventory/UIInventorySlot.h"
#include "UI/Element/Skill/UISkillSlot.h"
#include "Subsystems/InputActions/InputActions.h"
#include "Windows/DX/UISprite.h"

UIQuickSlot::UIQuickSlot(const std::wstring& name) :
    UIContainer(name),
    scancode_(Scancode::kKeyUnknown),
    key_type_(KeyType::kNone),
    action_(0)
{
    SetSize({ 32.f, 32.f });
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSize(GetSize());
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    icon_ = AddChild<UIImage>(UIImage::StaticClass(), L"Icon");
    icon_->SetRelativePosition(Math::Vector2(4.f, 4.f));
    icon_->SetSize(GetSize() - Math::Vector2(8.f, 8.f));
    icon_->SetIgnoreRayCast(true);
    icon_->SetActive(false);

    key_name_text_ = AddChild<UIText>(UIText::StaticClass(), L"KeyNameText");
    key_name_text_->SetRelativePosition(Math::Vector2(2.f, 2.f));
    key_name_text_->SetSize(GetSize() - Math::Vector2(4.f, 4.f));
    key_name_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    key_name_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    key_name_text_->SetColor(Math::Color::White);
    key_name_text_->SetIgnoreRayCast(true);

    count_text_ = AddChild<UIText>(UIText::StaticClass(), L"CountText");
    count_text_->SetRelativePosition(Math::Vector2(2.f, 2.f));
    count_text_->SetSize(GetSize() - Math::Vector2(4.f, 4.f));
    count_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    count_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    count_text_->SetColor(Math::Color::White);
    count_text_->SetText(L"");
    count_text_->SetIgnoreRayCast(true);
}

void UIQuickSlot::SetScancode(Scancode scancode)
{
    scancode_ = scancode;
    if (IsInitialized())
        key_name_text_->SetText(ScancodeToKeyName(scancode_));
}

void UIQuickSlot::Init()
{
    UIContainer::Init();

    key_name_text_->SetText(ScancodeToKeyName(scancode_));
}

bool UIQuickSlot::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIQuickSlot::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    return true;
}

bool UIQuickSlot::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

bool UIQuickSlot::OnDrop(const Math::Vector2& position, UIElement* target)
{
    if (auto* inventory_slot = dynamic_cast<UIInventorySlot*>(target))
    {
        return true;
    }

    if (auto* skill_slot = dynamic_cast<UISkillSlot*>(target))
    {
        if (skill_slot->GetSkillID() == 0) return false;

        InputActions::Get()->Bind(scancode_, KeyType::kSkill, static_cast<int32_t>(skill_slot->GetSkillID()));

        key_type_ = KeyType::kSkill;
        action_ = static_cast<int32_t>(skill_slot->GetSkillID());

        icon_->SetSprite(skill_slot->GetIcon()->GetSprite(), skill_slot->GetIcon()->GetFrameIndex());
        icon_->SetDrawMode(skill_slot->GetIcon()->GetDrawMode());
        icon_->SetColor(skill_slot->GetIcon()->GetColor());
        icon_->SetActive(true);

        return true;
    }

    return false;
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
