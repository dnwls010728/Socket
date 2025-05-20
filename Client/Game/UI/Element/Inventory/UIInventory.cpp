#include "pch.h"
#include "UIInventory.h"

#include "UIInventorySlot.h"
#include "Math/Color.h"
#include "Windows/DX/Renderer.h"

UIInventory::UIInventory(const std::wstring& name) :
    UIContainer(name),
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
            UIInventorySlot* slot = AddChild<UIInventorySlot>(UIInventorySlot::StaticClass(), L"Slot");
            slot->SetRelativePosition({ 8 + j * 36.f, 24 + i * 36.f });
            slots_[j + i * 4 + 1] = slot;
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
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
