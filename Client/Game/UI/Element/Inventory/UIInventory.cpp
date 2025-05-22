#include "pch.h"
#include "UIInventory.h"

#include "UIInventorySlot.h"
#include "DirectXTK/SimpleMath.h"
#include "imgui/imgui.h"
#include "Inventory/Inventory.h"
#include "Math/Color.h"
#include "UI/Element/UIScrollBox.h"
#include "Windows/DX/Renderer.h"

UIInventory::UIInventory(const std::wstring& name) :
    UIContainer(name),
    scroll_box_(nullptr),
    slots_(),
    t_color_(nullptr),
    inventory_(nullptr)
{
    size_ = { 158.f, 224.f };
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

void UIInventory::InitInventory(Inventory* inventory)
{
    if (!inventory) return;
    inventory_ = inventory;

    for (uint32_t i = 0; i < 40; ++i)
    {
        UpdateSlot(i + 1);
    }

    UpdateColor(inventory->GetColor());
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

    UIText* t_title = AddChild<UIText>(UIText::StaticClass(), L"Title");
    t_title->SetRelativePosition({ 8.f, 0.f });
    t_title->SetSize({ 142.f, 20.f });
    t_title->SetColor(Math::Color::White);
    t_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    t_title->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    t_title->SetText(L"인벤토리");
    t_title->SetIgnoreRayCast(true);

    scroll_box_ = AddChild<UIScrollBox>(UIScrollBox::StaticClass(), L"SlotContainer");
    scroll_box_->SetRelativePosition({ 8.f, 24.f });
    scroll_box_->SetSize({ 142.f, 176.f });

    for (uint32_t i = 0; i < 10; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            UIInventorySlot* slot = scroll_box_->AddChild<UIInventorySlot>(UIInventorySlot::StaticClass(), L"Slot");
            slot->SetRelativePosition({ j * 36.f, i * 36.f });
            
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

    SetActive(false);
}

void UIInventory::Tick(float delta_time)
{
    UIContainer::Tick(delta_time);

    static float a = 0.f;
    if (ImGui::SliderFloat("Color", &a, 0.f, 1.f))
    {
        scroll_box_->SetScrollY(a);
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
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
