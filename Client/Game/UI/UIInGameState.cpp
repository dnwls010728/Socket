#include "pch.h"
#include "UIInGameState.h"

#include "Element/UIStatusBar.h"
#include "Element/Inventory/UIInventory.h"
#include "Subsystems/PlayerSubsystem.h"

UIInGameState::UIInGameState() :
    inventory_(nullptr)
{
}

void UIInGameState::Init()
{
    UIState::Init();

    inventory_ = AddElement<UIInventory>(UIInventory::StaticClass(), L"Inventory");
    inventory_->InitInventory(PlayerSubsystem::Get()->GetInventory());

    AddElement<UIStatusBar>(UIStatusBar::StaticClass(), L"StatusBar");
}

bool UIInGameState::OnKey(uint16_t key_code, bool is_pressed)
{
    if (key_code == 'I' && is_pressed)
    {
        inventory_->SetActive(!inventory_->IsActive());
    }
    
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
