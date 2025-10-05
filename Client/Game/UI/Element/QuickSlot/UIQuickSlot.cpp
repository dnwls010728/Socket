#include "pch.h"
#include "UIQuickSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/Inventory/UIInventorySlot.h"
#include "UI/Element/Skill/UISkillSlot.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/Player/SkillManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "Subsystems/InputActions/InputActions.h"
#include "Windows/DX/UISprite.h"
#include "Math/Math.h"
#include <CustomPacket.h>

#include <iomanip>
#include <sstream>

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

    cooldown_text_ = AddChild<UIText>(UIText::StaticClass(), L"CooldownText");
    cooldown_text_->SetRelativePosition(icon_->GetRelativePosition());
    cooldown_text_->SetSize(icon_->GetSize());
    cooldown_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    cooldown_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    cooldown_text_->SetColor(Math::Color::White);
    cooldown_text_->SetIgnoreRayCast(true);
    cooldown_text_->SetText(L"");
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

    ApplyMapping(InputActions::Get()->GetMapping(scancode_));
}

void UIQuickSlot::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);
    UpdateCooldownVisual();
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

        KeyBindRequest request;
        request.scancode = static_cast<uint32_t>(scancode_);
        request.type = static_cast<uint8_t>(KeyType::kSkill);
        request.action = action_;
        SessionSubsystem::Get()->SendPacket(request);

        ApplySkillMapping(skill_slot->GetSkillID());
        UpdateCooldownVisual();

        return true;
    }

    return false;
}

bool UIQuickSlot::OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp)
{
    if (UIContainer::OnMouseButton(position, button, is_pressed, timestamp))
        return true;

    if (!is_pressed && button == MouseButton::kRight)
    {
        if (key_type_ != KeyType::kNone)
        {
            InputActions::Get()->Unbind(scancode_);
            ClearMapping();

            KeyUnbindRequest request;
            request.scancode = static_cast<uint32_t>(scancode_);
            SessionSubsystem::Get()->SendPacket(request);

            return true;
        }
    }

    return false;
}

void UIQuickSlot::ApplyMapping(const InputActions::Mapping& mapping)
{
    key_type_ = static_cast<KeyType>(mapping.type);
    action_ = mapping.action;

    switch (key_type_)
    {
    case KeyType::kSkill:
        ApplySkillMapping(static_cast<uint32_t>(action_));
        break;
    case KeyType::kItem:
    case KeyType::kMenu:
    case KeyType::kNone:
    default:
        ClearMapping();
        break;
    }

    UpdateCooldownVisual();
}

void UIQuickSlot::ApplySkillMapping(uint32_t skill_id)
{
    if (skill_id == 0)
    {
        ClearMapping();
        return;
    }

    if (!icon_->IsActive())
    {
        UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
        icon_->SetSprite(panel_sprite, L"Panel_0");
        icon_->SetDrawMode(UIImage::DrawMode::kSliced);
        icon_->SetColor(Math::Color(80, 120, 200, 255));
        icon_->SetActive(true);
    }

    PlayerSubsystem* player = PlayerSubsystem::Get();
    SkillManager* skill_manager = player ? player->GetSkillManager() : nullptr;
    if (skill_manager && !skill_manager->HasSkill(skill_id))
    {
        ClearMapping();
        return;
    }

    key_type_ = KeyType::kSkill;
    action_ = static_cast<int32_t>(skill_id);
}

void UIQuickSlot::ClearMapping()
{
    key_type_ = KeyType::kNone;
    action_ = 0;

    icon_->SetSprite(nullptr, L"");
    icon_->SetActive(false);
    
    cooldown_text_->SetText(L"");
    count_text_->SetText(L"");
}

void UIQuickSlot::UpdateCooldownVisual()
{
    auto set_no_cooldown = [&]()
    {
        cooldown_text_->SetText(L"");
    };

    if (key_type_ != KeyType::kSkill || action_ == 0)
    {
        set_no_cooldown();
        return;
    }

    PlayerSubsystem* player = PlayerSubsystem::Get();
    SkillManager* skill_manager = player->GetSkillManager();
    if (!skill_manager)
    {
        set_no_cooldown();
        return;
    }

    uint32_t skill_id = static_cast<uint32_t>(action_);
    float cooldown = skill_manager->GetCooldown(skill_id);
    float remaining = skill_manager->GetCoolDownLeft(skill_id);

    if (cooldown <= 0.f || remaining <= 0.f)
    {
        set_no_cooldown();
        return;
    }

    std::wstringstream stream;
    stream << std::fixed << std::setprecision(1) << remaining;
    cooldown_text_->SetText(stream.str());
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
