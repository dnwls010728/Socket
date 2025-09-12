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

    inline static constexpr std::array<Scancode, 16> kDefaultScancodes =
    {
        Scancode::kKey1, Scancode::kKey2, Scancode::kKey3, Scancode::kKey4,
        Scancode::kKeyQ, Scancode::kKeyW, Scancode::kKeyE, Scancode::kKeyR,
        Scancode::kKeyA, Scancode::kKeyS, Scancode::kKeyD, Scancode::kKeyF,
        Scancode::kKeyZ, Scancode::kKeyX, Scancode::kKeyC, Scancode::kKeyV
    };
    
};
