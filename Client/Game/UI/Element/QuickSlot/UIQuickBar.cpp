#include "pch.h"
#include "UIQuickBar.h"

#include <CustomPacket.h>

#include "UIQuickSlot.h"
#include "Subsystems/SessionSubsystem.h"

UIQuickBar::UIQuickBar(const std::wstring& name) :
    UIContainer(name),
    quick_slots_(),
    scancode_to_slot_(),
    rows_(4),
    cols_(4)
{
    for (int32_t y = 0; y < rows_; y++)
    {
        for (int32_t x = 0; x < cols_; x++)
        {
            UIQuickSlot* slot = AddChild<UIQuickSlot>(UIQuickSlot::StaticClass(), L"QuickSlot_" + std::to_wstring(y * cols_ + x));
            slot->SetOwner(this);
            slot->SetRelativePosition({ static_cast<float>(x * 36), static_cast<float>(y * 36) });
            slot->SetScancode(kDefaultScancodes[y * cols_ + x]);
            quick_slots_.push_back(slot);
        }
    }

    float width = static_cast<float>(cols_ * 36 - 4);
    float height = static_cast<float>(rows_ * 36 - 4);
    
    SetSize({ width, height });
}

UIQuickSlot* UIQuickBar::FindSlot(KeyType key_type, int32_t action)
{
    for (auto it = quick_slots_.begin(); it != quick_slots_.end(); ++it)
    {
        if ((*it)->GetKeyType() == key_type && (*it)->GetAction() == action)
            return *it;
    }
    return nullptr;
}

void UIQuickBar::UnboundSlot(Scancode scancode)
{
    
}

void UIQuickBar::UnboundSlot(KeyType key_type, int32_t action)
{
    while (UIQuickSlot* slot = FindSlot(key_type, action))
    {
        InputActions::Get()->Unbind(slot->GetScancode());
            
        KeyUnbindRequest unbind_request;
        unbind_request.scancode = static_cast<uint32_t>(slot->GetScancode());
        SessionSubsystem::Get()->SendPacket(unbind_request);

        slot->ClearMapping();
    }
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIQuickBar>("UIQuickBar")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
