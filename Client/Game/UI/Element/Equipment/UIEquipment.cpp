#include "pch.h"
#include "UIEquipment.h"

#include "UIEquipmentSlot.h"
#include "Asset/AssetManager.h"
#include "Inventory/Inventory.h"
#include "Subsystems/PlayerSubsystem.h"
#include "UI/Element/UIImage.h"
#include "UI/Element/UIText.h"
#include "Windows/DX/UISprite.h"

UIEquipment::UIEquipment(const std::wstring& name) :
    UIContainer(name),
    slots_(),
    timer_(0.f),
    frame_index_(0)
{
    SetSize({164.f, 246.f});
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");
    
    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);

    character_ = AddChild<UIImage>(UIImage::StaticClass(), L"Character");
    character_->SetRelativePosition({ 10.f, 32.f });
    character_->SetSize({ 144.f, 126.f });
    character_->SetSprite(character_sprite, L"UIPlayerSheet_0");
    character_->SetIgnoreRayCast(true);
    
    UIText* t_title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    t_title->SetRelativePosition({ 8.f, 0.f });
    t_title->SetSize({ 142.f, 20.f });
    t_title->SetColor(Math::Color::White);
    t_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    t_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_title->SetText(L"캐릭터");
    t_title->SetIgnoreRayCast(true);

    UIEquipmentSlot* hat_slot = AddChild<UIEquipmentSlot>(UIEquipmentSlot::StaticClass(), L"HatSlot");
    hat_slot->SetRelativePosition({ 124.f, 32.f });
    hat_slot->SetSlotID(static_cast<uint8_t>(EquipSlot::kHat));
    
    UIEquipmentSlot* top_slot = AddChild<UIEquipmentSlot>(UIEquipmentSlot::StaticClass(), L"TopSlot");
    top_slot->SetRelativePosition({ 124.f, 68.f });
    top_slot->SetSlotID(static_cast<uint8_t>(EquipSlot::kTop));
    
    UIEquipmentSlot* weapon_slot = AddChild<UIEquipmentSlot>(UIEquipmentSlot::StaticClass(), L"WeaponSlot");
    weapon_slot->SetRelativePosition({ 8.f, 32.f });
    weapon_slot->SetSlotID(static_cast<uint8_t>(EquipSlot::kWeapon));

    slots_[hat_slot->GetSlotID()] = hat_slot;
    slots_[top_slot->GetSlotID()] = top_slot;
    slots_[weapon_slot->GetSlotID()] = weapon_slot;
}

void UIEquipment::UpdateSlot(uint32_t slot_id) const
{
    auto inventory = PlayerSubsystem::Get()->GetInventory();
    if (!inventory) return;
    
    if (uint32_t item_id = inventory->GetItemID(InventoryType::kEquipped, slot_id))
        slots_[slot_id]->UpdateSlot(item_id);
    else slots_[slot_id]->ResetSlot();
}

void UIEquipment::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();
    
    PublisherSubsystem* subsystem = PublisherSubsystem::Get();
    subsystem->Subscribe(PublisherSubsystem::EventType::kItemAdded, this, &UIEquipment::OnEvent);
    subsystem->Subscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIEquipment::OnEvent);
    
    for (uint32_t i = 1; i < 4; ++i)
    {
        UpdateSlot(i);
    }

    Math::Color body_color = Math::Color::HexToColor(PlayerSubsystem::Get()->GetBodyColor());
    character_->SetColor(body_color);
    
}

void UIEquipment::Uninit()
{
    UIContainer::Uninit();
    
    PublisherSubsystem* subsystem = PublisherSubsystem::Get();
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kItemAdded, this, &UIEquipment::OnEvent);
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIEquipment::OnEvent);
    
}

void UIEquipment::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);
    
    timer_ += delta_time;
    if (timer_ >= 1.f / 10.f)
    {
        UISprite* character_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\UIPlayerSheet.png");

        frame_index_ = (frame_index_ + 1) % 7;
        character_->SetSprite(character_sprite, L"UIPlayerSheet_" + std::to_wstring(frame_index_));
        
        timer_ = 0.f;
    }
}

bool UIEquipment::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIEquipment::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    Math::Vector2 new_position = GetRelativePosition() + delta;
    SetRelativePosition(new_position);
    return true;
}

bool UIEquipment::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

void UIEquipment::OnEvent(const EventData& data)
{
    if (const auto* item_added = dynamic_cast<const ItemAddedData*>(&data))
    {
        if (item_added->inventory_type != InventoryType::kEquipped) return;
        UpdateSlot(item_added->slot_id);
    }
    else if (const auto* item_removed = dynamic_cast<const ItemRemovedData*>(&data))
    {
        if (item_removed->inventory_type != InventoryType::kEquipped) return;
        UpdateSlot(item_removed->slot_id);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIEquipment>("UIEquipment")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
