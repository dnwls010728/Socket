#include "pch.h"
#include "UIInventory.h"

UIInventory::UIInventory()
{
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
