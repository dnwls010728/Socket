#include "pch.h"
#include "UILoginState.h"

#include "Element/Inventory/UIInventory.h"

UILoginState::UILoginState()
{
}

void UILoginState::Init()
{
    UIState::Init();

    AddElement<UIInventory>(UIInventory::StaticClass());

}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UILoginState>("UILoginState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
