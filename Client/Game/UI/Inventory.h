#pragma once
#include "UI/Widget.h"

namespace UI
{
    class Inventory : public Widget
    {
        SHADER_CLASS_HELPER(Inventory)
        GENERATED_BODY(Inventory, Widget)
        
    public:
        Inventory(const std::wstring& name);
        virtual ~Inventory() override = default;

    protected:
        virtual void Render(Renderer* renderer, WindowsWindow* window) override;

    private:
        int16_t slot_row;
        int16_t slot_col;
        int16_t max_slots;
    
    };
}
