#include "pch.h"
#include "UIInGameState.h"

#include "Element/UIChatBar.h"
#include "Element/UIMiniMap.h"
#include "Element/UIStatusBar.h"
#include "Element/Inventory/UIInventory.h"
#include "Element/Inventory/UIItemTooltip.h"
#include "Subsystems/PlayerSubsystem.h"

UIInGameState::UIInGameState() :
    inventory_(nullptr),
    item_tooltip_(nullptr)
{
}

void UIInGameState::Init()
{
    AddElement<UIMiniMap>(UIMiniMap::StaticClass(), L"MiniMap");
    
    char_bar_ = AddElement<UIChatBar>(UIChatBar::StaticClass(), L"ChatBar");
    char_bar_->SetAbsolutePosition({0.f, 684.f});
    
    inventory_ = AddElement<UIInventory>(UIInventory::StaticClass(), L"Inventory");
    inventory_->SetActive(false);
    
    AddElement<UIStatusBar>(UIStatusBar::StaticClass(), L"StatusBar");

    item_tooltip_ = AddElement<UIItemTooltip>(UIItemTooltip::StaticClass(), L"ItemTooltip");
    item_tooltip_->SetAbsolutePosition({ 100.f, 100.f });
    item_tooltip_->SetSize({ 322.f, 122.f });
    item_tooltip_->SetActive(false);
    item_tooltip_->SetIgnoreRayCast(true);
    
    UIState::Init();
}

bool UIInGameState::OnKey(uint16_t key_code, bool is_pressed)
{
    bool is_handled = UIState::OnKey(key_code, is_pressed);
    
    if (!is_handled && is_pressed)
    {
        if (key_code == 'I' && !IsEditingText())
        {
            inventory_->SetActive(!inventory_->IsActive());
            is_handled = true;
        }

        if (key_code == VK_RETURN && !IsEditingText())
        {
            char_bar_->FocusInput();
            is_handled = true;
        }
    }
    
    return is_handled;
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
