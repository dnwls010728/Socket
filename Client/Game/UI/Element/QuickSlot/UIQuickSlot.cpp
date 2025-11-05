#include "pch.h"
#include "UIQuickSlot.h"

#include "Asset/AssetManager.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/Inventory/UIInventorySlot.h"
#include "UI/Element/Skill/UISkillSlot.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Inventory/Inventory.h"
#include "Subsystems/Player/SkillManager.h"
#include "Subsystems/SessionSubsystem.h"
#include "Subsystems/InputActions/InputActions.h"
#include "Windows/DX/UISprite.h"
#include "Math/Math.h"
#include <CustomPacket.h>

#include <iomanip>
#include <sstream>

#include "UIQuickBar.h"
#include "Subsystems/DataSubsystem.h"

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
    icon_->SetDrawMode(UIImage::DrawMode::kSimple);
    icon_->SetColor(Math::Color::White);
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
    UpdateItemCountVisual();
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
        uint32_t item_id = inventory_slot->GetItemID();
        if (item_id == 0)
            return false;

        InventoryType inventory_type = static_cast<InventoryType>(item_id / 100000);
        if (inventory_type != InventoryType::kUse)
            return false;

        InputActions* input_actions = InputActions::Get();
        if (!input_actions)
            return false;

        KeyType key_type = KeyType::kItem;
        int32_t action = static_cast<int32_t>(item_id);
        if (owner_) owner_->UnboundSlot(key_type, action);
        
        input_actions->Bind(scancode_, KeyType::kItem, static_cast<int32_t>(item_id));
        ApplyItemMapping(item_id);
        
        KeyBindRequest request;
        request.scancode = static_cast<uint32_t>(scancode_);
        request.type = static_cast<uint8_t>(KeyType::kItem);
        request.action = static_cast<int32_t>(item_id);
        SessionSubsystem::Get()->SendPacket(request);
    }
    else if (auto* skill_slot = dynamic_cast<UISkillSlot*>(target))
    {
        if (skill_slot->GetSkillID() == 0) return false;

        InputActions* input_actions = InputActions::Get();
        if (!input_actions)
            return false;

        uint32_t new_skill_id = skill_slot->GetSkillID();

        KeyType key_type = KeyType::kSkill;
        int32_t action = static_cast<int32_t>(new_skill_id);
        if (owner_) owner_->UnboundSlot(key_type, action);
        
        input_actions->Bind(scancode_, key_type, action);
        
        ApplySkillMapping(skill_slot->GetSkillID());
        
        KeyBindRequest request;
        request.scancode = static_cast<uint32_t>(scancode_);
        request.type = static_cast<uint8_t>(KeyType::kSkill);
        request.action = action_;
        SessionSubsystem::Get()->SendPacket(request);
    }
    else
    {
        return false;
    }
    
    return true;
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
        ApplyItemMapping(static_cast<uint32_t>(action_));
        break;
    case KeyType::kMenu:
    case KeyType::kNone:
    default:
        ClearMapping();
        break;
    }

    UpdateCooldownVisual();
    UpdateItemCountVisual();
}

void UIQuickSlot::ApplySkillMapping(uint32_t skill_id)
{
    if (skill_id == 0)
    {
        ClearMapping();
        return;
    }

    PlayerSubsystem* player = PlayerSubsystem::Get();
    SkillManager* skill_manager = player ? player->GetSkillManager() : nullptr;
    if (skill_manager && !skill_manager->HasSkill(skill_id))
    {
        ClearMapping();
        return;
    }

    const SkillData* skill_data = DataSubsystem::Get()->GetSkill(skill_id);
    if (!skill_data)
    {
        ClearMapping();
        return;
    }

    UISprite* skill_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\SkillIconSet.png");
    if (skill_sprite && !skill_data->icon.empty())
    {
        icon_->SetSprite(skill_sprite, skill_data->icon);
        if (!icon_->IsActive())
            icon_->SetActive(true);
    }
    else
    {
        icon_->SetSprite(nullptr, L"");
        icon_->SetActive(false);
    }

    key_type_ = KeyType::kSkill;
    action_ = static_cast<int32_t>(skill_id);

    count_text_->SetText(L"");

    UpdateCooldownVisual();
}

void UIQuickSlot::ApplyItemMapping(uint32_t item_id)
{
    if (item_id == 0)
    {
        ClearMapping();
        return;
    }

    InventoryType inventory_type = static_cast<InventoryType>(item_id / 100000);
    if (inventory_type != InventoryType::kUse)
    {
        ClearMapping();
        return;
    }
    
    const ItemData* item_data = DataSubsystem::Get()->GetItem(item_id);
    if (!item_data) return;
    
    UISprite* item_sprite = AssetManager::Get()->Load<UISprite>(item_data->ui_icon.path);
    int32_t frame_index = item_data->ui_icon.index;

    if (!item_sprite)
    {
        static UISprite* kMissing = AssetManager::Get()->Load<UISprite>(L"UI\\Item\\Missing.png");
        item_sprite = kMissing;
        frame_index = 0;
    }

    icon_->SetSprite(item_sprite, frame_index);
    
    if (!icon_->IsActive())
        icon_->SetActive(true);
    
    key_type_ = KeyType::kItem;
    action_ = static_cast<int32_t>(item_id);

    cooldown_text_->SetText(L"");
    UpdateItemCountVisual();
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

void UIQuickSlot::UpdateItemCountVisual()
{
    if (key_type_ != KeyType::kItem || action_ == 0)
    {
        count_text_->SetText(L"");
        return;
    }

    PlayerSubsystem* player = PlayerSubsystem::Get();
    Inventory* inventory = player ? player->GetInventory() : nullptr;
    if (!inventory)
    {
        count_text_->SetText(L"");
        return;
    }

    uint32_t item_id = static_cast<uint32_t>(action_);
    InventoryType inventory_type = static_cast<InventoryType>(item_id / 100000);
    if (inventory_type == InventoryType::kNone)
    {
        count_text_->SetText(L"");
        return;
    }

    int32_t total_count = inventory->GetTotalItemCount(inventory_type, item_id);
    if (total_count <= 0)
    {
        count_text_->SetText(L"0");
        return;
    }

    count_text_->SetText(std::to_wstring(total_count));
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
