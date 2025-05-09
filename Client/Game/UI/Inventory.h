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
    
    };
}
