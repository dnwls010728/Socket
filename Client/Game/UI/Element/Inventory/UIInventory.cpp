#include "pch.h"
#include "UIInventory.h"

#include "UIInventorySlot.h"
#include "Inventory/InventoryManager.h"
#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UIInventory::UIInventory() :
    slots_()
{
    size_ = { 158.f, 224.f };
}

void UIInventory::UpdateSlots(InventoryManager* inventory)
{
    if (!inventory) return;
    for (uint32_t i = 0; i < 20; ++i)
    {
        int32_t item_id = inventory->GetItemID(i);
        if (item_id == -1) continue;

        int16_t count = inventory->GetItemCount(i);
        slots_[i]->UpdateSlot(item_id, count);
    }
}

void UIInventory::Init()
{
    UIContainer::Init();

    for (uint32_t i = 0; i < 5; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            UIInventorySlot* slot = AddChild<UIInventorySlot>(UIInventorySlot::StaticClass());
            slot->SetRelativePosition({ 8 + j * 36.f, 24 + i * 36.f });
            slots_[j + i * 4] = slot;
        }
    }
}

void UIInventory::Render()
{
    Math::Vector2 parent_position = parent_ ? parent_->GetRelativePosition() : Math::Vector2::Zero();
    Math::Vector2 position = parent_position + position_;
    Renderer::Get()->DrawBox(position, size_, Math::Color::Red);
    
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

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInventory>("UIInventory")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
