#include "pch.h"
#include "UIInventory.h"

#include "Scancode.h"
#include "UIInventorySlot.h"
#include "UIItemTooltip.h"
#include "Asset/AssetManager.h"
#include "Inventory/Inventory.h"
#include "Math/Color.h"
#include "Subsystems/PlayerSubsystem.h"
#include "UI/UIInGameState.h"
#include "UI/Element/UIButton.h"
#include "UI/Element/UIScrollBox.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIInventory::UIInventory(const std::wstring& name) :
    UIContainer(name),
    slots_(),
    color_text_(nullptr),
    inventory_(nullptr),
    tab_(InventoryType::kEquip)
{
    SetSize({164.f, 246.f});
    
    UISprite* panel_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\Panel.png");
    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

    background_ = AddChild<UIImage>(UIImage::StaticClass(), L"Background");
    background_->SetSprite(panel_sprite, L"Panel_0");
    background_->SetDrawMode(UIImage::DrawMode::kSliced);
    background_->SetIgnoreRayCast(true);
    
    UIText* t_title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    t_title->SetRelativePosition({ 8.f, 0.f });
    t_title->SetSize({ 142.f, 20.f });
    t_title->SetColor(Math::Color::White);
    t_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    t_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_title->SetText(L"인벤토리");
    t_title->SetIgnoreRayCast(true);

    UIButton* equip_button = AddChild<UIButton>(UIButton::StaticClass(), L"EquipButton");
    equip_button->SetRelativePosition({ 8.f, 20.f });
    equip_button->SetSize({ 35.f, 22.f });
    equip_button->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    equip_button->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    equip_button->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    equip_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    equip_button->SetTextColor(Math::Color::White);
    equip_button->SetText(L"장비");
    equip_button->OnClick([&]()
    {
        if (tab_ == InventoryType::kEquip) return;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::White);
        
        tab_ = InventoryType::kEquip;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
        
        for (uint32_t i = 0; i < 128; ++i)
        {
            UpdateSlot(i + 1);
        }
    });
    
    UIButton* use_button = AddChild<UIButton>(UIButton::StaticClass(), L"UseButton");
    use_button->SetRelativePosition({ 43.f, 20.f });
    use_button->SetSize({ 35.f, 22.f });
    use_button->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    use_button->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    use_button->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    use_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    use_button->SetTextColor(Math::Color::White);
    use_button->SetText(L"소비");
    use_button->OnClick([&]()
    {
        if (tab_ == InventoryType::kUse) return;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::White);
        
        tab_ = InventoryType::kUse;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
        
        for (uint32_t i = 0; i < 128; ++i)
        {
            UpdateSlot(i + 1);
        }
    });
    
    UIButton* etc_button = AddChild<UIButton>(UIButton::StaticClass(), L"EtcButton");
    etc_button->SetRelativePosition({ 78.f, 20.f });
    etc_button->SetSize({ 35.f, 22.f });
    etc_button->SetSprite(UIButton::State::kNormal, button_sprite, L"ButtonSheet_0");
    etc_button->SetSprite(UIButton::State::kHover, button_sprite, L"ButtonSheet_1");
    etc_button->SetSprite(UIButton::State::kPressed, button_sprite, L"ButtonSheet_2");
    etc_button->SetSprite(UIButton::State::kDisabled, button_sprite, L"ButtonSheet_3");
    etc_button->SetTextColor(Math::Color::White);
    etc_button->SetText(L"기타");
    etc_button->OnClick([&]()
    {
        if (tab_ == InventoryType::kEtc) return;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::White);
        
        tab_ = InventoryType::kEtc;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
        
        for (uint32_t i = 0; i < 128; ++i)
        {
            UpdateSlot(i + 1);
        }
    });

    tab_buttons_[static_cast<uint8_t>(InventoryType::kEquip)] = equip_button;
    tab_buttons_[static_cast<uint8_t>(InventoryType::kUse)] = use_button;
    tab_buttons_[static_cast<uint8_t>(InventoryType::kEtc)] = etc_button;

    scroll_box_ = AddChild<UIScrollBox>(UIScrollBox::StaticClass(), L"ScrollBox");
    scroll_box_->SetRelativePosition({ 8.f, 48.f });
    scroll_box_->SetSize({ 150.f, 180.f });
    scroll_box_->SetScrollStep(36.f);

    UIContainer* content = scroll_box_->AddItem<UIContainer>(UIContainer::StaticClass(), L"Content");
    content->SetSize({ 144.f, 1152.f });

    for (uint32_t i = 0; i < 32; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            UIInventorySlot* slot = content->AddChild<UIInventorySlot>(UIInventorySlot::StaticClass(), L"Slot");
            slot->SetRelativePosition({ j * 36.f, i * 36.f });

            slot->SetUIInventory(this);
            slot->SetSlotID(i * 4 + j + 1);
            slots_.push_back(slot);
        }
    }
    
    color_text_ = AddChild<UIText>(UIText::StaticClass(), L"Color");
    color_text_->SetRelativePosition({ 8.f, 224.f });
    color_text_->SetSize({ 142.f, 20.f });
    color_text_->SetColor(Math::Color::White);
    color_text_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    color_text_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    color_text_->SetText(L"0Color");
    color_text_->SetIgnoreRayCast(true);
    
    dragging_item_ = AddChild<UIImage>(UIImage::StaticClass(), L"DraggingItem");
    dragging_item_->SetSize({32.f, 32.f});
    dragging_item_->SetActive(false);
    dragging_item_->SetIgnoreRayCast(true);

}

void UIInventory::SetActive(bool active)
{
    UIContainer::SetActive(active);

    if (!active)
    {
        if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
        {
            if (auto* element = state->GetItemTooltip())
            {
                if (element->IsActive()) element->SetActive(false);
            }
        }
    }
}

void UIInventory::UpdateSlot(uint32_t slot_id) const
{
    if (!inventory_) return;
    if (uint32_t item_id = inventory_->GetItemID(tab_, slot_id))
    {
        uint32_t count = inventory_->GetItemCount(tab_, slot_id);
        slots_[slot_id - 1]->UpdateSlot(item_id, count);
    }
    else slots_[slot_id - 1]->ResetSlot();
}

void UIInventory::UpdateColor(uint32_t color) const
{
    std::wstring color_str = std::to_wstring(color);
    for (uint32_t i = color_str.size(); i > 3; i -= 3)
    {
        color_str.insert(i - 3, L",");
    }
    
    color_text_->SetText(color_str + L" 컬러");
}

void UIInventory::Init()
{
    background_->SetSize(GetSize());
    
    UIContainer::Init();

    PublisherSubsystem* subsystem = PublisherSubsystem::Get();
    subsystem->Subscribe(PublisherSubsystem::EventType::kItemAdded, this, &UIInventory::OnEvent);
    subsystem->Subscribe(PublisherSubsystem::EventType::kItemCountChanged, this, &UIInventory::OnEvent);
    subsystem->Subscribe(PublisherSubsystem::EventType::kItemMoved, this, &UIInventory::OnEvent);
    subsystem->Subscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIInventory::OnEvent);
    subsystem->Subscribe(PublisherSubsystem::EventType::kColorUpdated, this, &UIInventory::OnEvent);

    inventory_ = PlayerSubsystem::Get()->GetInventory();
    for (uint32_t i = 0; i < 128; ++i)
    {
        UpdateSlot(i + 1);
    }

    UpdateColor(inventory_->GetColor());
    tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
    
}

void UIInventory::Uninit()
{
    UIContainer::Uninit();
    
    PublisherSubsystem* subsystem = PublisherSubsystem::Get();
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kItemAdded, this, &UIInventory::OnEvent);
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kItemCountChanged, this, &UIInventory::OnEvent);
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kItemMoved, this, &UIInventory::OnEvent);
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIInventory::OnEvent);
    subsystem->Unsubscribe(PublisherSubsystem::EventType::kColorUpdated, this, &UIInventory::OnEvent);
}

bool UIInventory::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIInventory::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    Math::Vector2 new_position = GetRelativePosition() + delta;
    SetRelativePosition(new_position);
    return true;
}

bool UIInventory::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

bool UIInventory::OnKey(uint32_t scancode, bool is_pressed)
{
    if (scancode != static_cast<uint32_t>(Scancode::kKeyTab) || !is_pressed) return false;

    uint8_t current_tab = static_cast<uint8_t>(tab_);
    tab_buttons_[current_tab]->SetTextColor(Math::Color::White);
    
    current_tab = (current_tab % (static_cast<uint8_t>(InventoryType::kCount) - 2)) + 1;
    tab_buttons_[current_tab]->SetTextColor(Math::Color::Yellow);
    
    tab_ = static_cast<InventoryType>(current_tab);
    
    for (uint32_t i = 0; i < 128; ++i)
    {
        UpdateSlot(i + 1);
    }
    
    return true;
}

void UIInventory::OnEvent(const EventData& data)
{
    if (const auto* item_added = dynamic_cast<const ItemAddedData*>(&data))
    {
        if (item_added->inventory_type != tab_) return;
        UpdateSlot(item_added->slot_id);
    }
    else if (const auto* count_changed = dynamic_cast<const ItemCountChangedData*>(&data))
    {
        if (count_changed->inventory_type != tab_) return;
        UpdateSlot(count_changed->slot_id);
    }
    else if (const auto* item_moved = dynamic_cast<const ItemMovedData*>(&data))
    {
        if (item_moved->first_inventory_type == tab_)
            UpdateSlot(item_moved->first_slot_index);
        
        if (item_moved->second_inventory_type == tab_)
            UpdateSlot(item_moved->second_slot_index);
    }
    else if (const auto* item_removed = dynamic_cast<const ItemRemovedData*>(&data))
    {
        if (item_removed->inventory_type != tab_) return;
        UpdateSlot(item_removed->slot_id);
    }
    else if (const auto* color_update = dynamic_cast<const ColorUpdateData*>(&data))
    {
        UpdateColor(color_update->color);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInventory>("UIInventory")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
