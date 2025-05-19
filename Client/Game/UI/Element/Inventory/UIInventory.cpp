#include "pch.h"
#include "UIInventory.h"

#include "UIInventorySlot.h"
#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UIInventory::UIInventory() :
    slots_()
{
    size_ = { 158.f, 224.f };
}

void UIInventory::Init()
{
    UIContainer::Init();

    for (uint32_t i = 0; i < 5; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            UIInventorySlot* slot = AddChild<UIInventorySlot>(UIInventorySlot::StaticClass());
            slot->SetPosition({ 8 + j * 36.f, 24 + i * 36.f });
            slot->SetSlotID(j + i * 4 + 1);
        }
    }
}

void UIInventory::Render()
{
    Math::Vector2 parent_position = parent_ ? parent_->GetPosition() : Math::Vector2::Zero();
    Math::Vector2 position = parent_position + position_;
    Renderer::Get()->DrawBox(position, size_, Math::Color::Red);
    
    UIContainer::Render();
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
