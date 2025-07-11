#include "pch.h"
#include "UIInventory.h"

#include "UIInventorySlot.h"
#include "Inventory/Inventory.h"
#include "Math/Color.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "Windows/DX/Renderer.h"

UIInventory::UIInventory(const std::wstring& name) :
    UIContainer(name),
    slots_(),
    t_color_(nullptr),
    inventory_(nullptr)
{
    size_ = { 158.f, 224.f };
    
    UIText* t_title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    t_title->SetRelativePosition({ 8.f, 0.f });
    t_title->SetSize({ 142.f, 20.f });
    t_title->SetColor(Math::Color::White);
    t_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    t_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_title->SetText(L"인벤토리");
    t_title->SetIgnoreRayCast(true);

    for (uint32_t i = 0; i < 5; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            UIInventorySlot* slot = AddChild<UIInventorySlot>(UIInventorySlot::StaticClass(), L"Slot");
            slot->SetRelativePosition({ 8.f + j * 36.f, 24.f + i * 36.f });
            
            slot->SetSlotID(i * 4 + j + 1);
            slots_.push_back(slot);
        }
    }
    
    t_color_ = AddChild<UIText>(UIText::StaticClass(), L"Color");
    t_color_->SetRelativePosition({ 8.f, 204.f });
    t_color_->SetSize({ 142.f, 20.f });
    t_color_->SetColor(Math::Color::White);
    t_color_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    t_color_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_color_->SetText(L"0Color");
    t_color_->SetIgnoreRayCast(true);

}

void UIInventory::UpdateSlot(uint32_t slot_index)
{
    if (!inventory_) return;
    if (uint32_t item_id = inventory_->GetItemID(slot_index))
    {
        uint32_t count = inventory_->GetItemCount(slot_index);
        slots_[slot_index - 1]->UpdateSlot(item_id, count);
    }
    else slots_[slot_index - 1]->UpdateSlot(0, 0);
}

void UIInventory::UpdateColor(uint32_t color)
{
    std::wstring color_str = std::to_wstring(color);
    for (uint32_t i = color_str.size(); i > 3; i -= 3)
    {
        color_str.insert(i - 3, L",");
    }
    
    t_color_->SetText(color_str + L" 컬러");
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
    SetActive(false);
    
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
