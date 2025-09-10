#pragma once
#include "Scancode.h"
#include "UI/UIContainer.h"

class UIQuickSlot;
class UIImage;

class UIQuickBar : public UIContainer
{
    GENERATED_BODY(UIQuickBar, UIContainer)
    
public:
    UIQuickBar(const std::wstring& name);
    virtual ~UIQuickBar() override = default;

private:
    std::vector<UIQuickSlot*> quick_slots_;
    std::unordered_map<Scancode, uint32_t> scancode_to_slot_;

    int32_t rows_;
    int32_t cols_;
    
};
