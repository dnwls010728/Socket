#include "pch.h"
#include "UIInventory.h"

#include "UIInventorySlot.h"
#include "Asset/AssetManager.h"
#include "Inventory/Inventory.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "UI/Element/UIButton.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UISprite.h"

UIInventory::UIInventory(const std::wstring& name) :
    UIContainer(name),
    slots_(),
    color_text_(nullptr),
    inventory_(nullptr),
    tab_(Inventory::Type::kEquip),
    scroll_offset_(0)
{
    size_ = { 158.f, 246.f };
    
    UIText* t_title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    t_title->SetRelativePosition({ 8.f, 0.f });
    t_title->SetSize({ 142.f, 20.f });
    t_title->SetColor(Math::Color::White);
    t_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    t_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_title->SetText(L"인벤토리");
    t_title->SetIgnoreRayCast(true);

    UISprite* button_sprite = AssetManager::Get()->Load<UISprite>(L"UI\\ButtonSheet.png");

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
        if (tab_ == Inventory::Type::kEquip) return;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::White);
        
        tab_ = Inventory::Type::kEquip;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
        
        for (uint32_t i = 0; i < 20; ++i)
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
        if (tab_ == Inventory::Type::kUse) return;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::White);
        
        tab_ = Inventory::Type::kUse;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
        
        for (uint32_t i = 0; i < 20; ++i)
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
        if (tab_ == Inventory::Type::kEtc) return;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::White);
        
        tab_ = Inventory::Type::kEtc;
        tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
        
        for (uint32_t i = 0; i < 20; ++i)
        {
            UpdateSlot(i + 1);
        }
    });

    tab_buttons_[static_cast<uint8_t>(Inventory::Type::kEquip)] = equip_button;
    tab_buttons_[static_cast<uint8_t>(Inventory::Type::kUse)] = use_button;
    tab_buttons_[static_cast<uint8_t>(Inventory::Type::kEtc)] = etc_button;

    for (uint32_t i = 0; i < 5; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            UIInventorySlot* slot = AddChild<UIInventorySlot>(UIInventorySlot::StaticClass(), L"Slot");
            slot->SetRelativePosition({ 8.f + j * 36.f, 48.f + i * 36.f });

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

}

void UIInventory::UpdateSlot(uint32_t slot_index)
{
    if (!inventory_) return;
    if (uint32_t item_id = inventory_->GetItemID(tab_, scroll_offset_ + slot_index))
    {
        uint32_t count = inventory_->GetItemCount(tab_, scroll_offset_ + slot_index);
        slots_[slot_index - 1]->UpdateSlot(item_id, count);
    }
    else slots_[slot_index - 1]->ResetSlot();
}

void UIInventory::UpdateColor(uint32_t color)
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
    UIContainer::Init();

    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kItemSwapped, this, &UIInventory::OnEvent);
    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kItemCountChanged, this, &UIInventory::OnEvent);
    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIInventory::OnEvent);

    inventory_ = PlayerSubsystem::Get()->GetInventory();
    for (uint32_t i = 0; i < 20; ++i)
    {
        UpdateSlot(i + 1);
    }

    UpdateColor(inventory_->GetColor());
    tab_buttons_[static_cast<uint8_t>(tab_)]->SetTextColor(Math::Color::Yellow);
    
}

void UIInventory::Uninit()
{
    UIContainer::Uninit();

    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kItemSwapped, this, &UIInventory::OnEvent);
    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kItemCountChanged, this, &UIInventory::OnEvent);
    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kItemRemoved, this, &UIInventory::OnEvent);
}

void UIInventory::Render()
{
    Renderer* renderer = Renderer::Get();
    renderer->DrawSolidRoundBox(GetAbsolutePosition(), size_, { 0, 0, 0, 128 });
    renderer->DrawRoundBox(GetAbsolutePosition(), size_, { 255, 255, 255, 255 });
    
    UIContainer::Render();
}

bool UIInventory::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    scroll_offset_ = Math::Max(0, scroll_offset_ + (delta.y * -1) * 4);
    for (uint32_t i = 0; i < 20; ++i)
    {
        UpdateSlot(i + 1);
    }

    Logger::Print(L"Scroll Delta: %f", delta.y);
    return true;
}

bool UIInventory::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIInventory::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    position_ += delta;
    return true;
}

bool UIInventory::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

bool UIInventory::OnKey(uint16_t key_code, bool is_pressed)
{
    if (key_code != VK_TAB || !is_pressed) return false;

    uint8_t current_tab = static_cast<uint8_t>(tab_);
    tab_buttons_[current_tab]->SetTextColor(Math::Color::White);
    
    current_tab = (current_tab % (static_cast<uint8_t>(Inventory::Type::kCount) - 1)) + 1;
    tab_buttons_[current_tab]->SetTextColor(Math::Color::Yellow);
    
    tab_ = static_cast<Inventory::Type>(current_tab);
    
    for (uint32_t i = 0; i < 20; ++i)
    {
        UpdateSlot(i + 1);
    }
    
    return true;
}

void UIInventory::OnEvent(const EventData& event_data)
{
    if (const ItemSwappedEventData* data = dynamic_cast<const ItemSwappedEventData*>(&event_data))
    {
        UpdateSlot(data->first_slot);
        UpdateSlot(data->second_slot);
    }
    else if (const ItemCountChangedEventData* data = dynamic_cast<const ItemCountChangedEventData*>(&event_data))
    {
        UpdateSlot(data->slot);
    }
    else if (const ItemRemovedEventData* data = dynamic_cast<const ItemRemovedEventData*>(&event_data))
    {
        UpdateSlot(data->slot);
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
