#include "pch.h"
#include "UIInGameState.h"

#include "Element/Inventory/UIInventory.h"

UIInGameState::UIInGameState() :
    inventory_(nullptr)
{
}

void UIInGameState::Init()
{
    UIState::Init();

    inventory_ = AddElement<UIInventory>(UIInventory::StaticClass());
}

bool UIInGameState::OnKey(uint16_t key_code, bool is_pressed)
{
    return UIState::OnKey(key_code, is_pressed);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInGameState>("UIInGameState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
