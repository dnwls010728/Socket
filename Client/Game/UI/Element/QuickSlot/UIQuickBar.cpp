#include "pch.h"
#include "UIQuickBar.h"

#include "UIQuickSlot.h"

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
            slot->SetRelativePosition({ static_cast<float>(x * 36), static_cast<float>(y * 36) });
            slot->SetScancode(kDefaultScancodes[y * cols_ + x]);
            quick_slots_.push_back(slot);
        }
    }

    float width = static_cast<float>(cols_ * 36 - 4);
    float height = static_cast<float>(rows_ * 36 - 4);
    
    SetSize({ width, height });
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
