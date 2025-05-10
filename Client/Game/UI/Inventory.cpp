#include "pch.h"
#include "Inventory.h"

UI::Inventory::Inventory(const std::wstring& name) :
    Widget(name),
    slot_row(4),
    slot_col(5),
    max_slots(0)
{
}

void UI::Inventory::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Inventory>("UI::Inventory")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
