#include "pch.h"
#include "Inventory.h"

UI::Inventory::Inventory(const std::wstring& name) :
    Widget(name)
{
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
