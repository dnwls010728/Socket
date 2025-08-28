#include "pch.h"
#include "UIInGameState.h"

#include "Scancode.h"
#include "UI/Element/UIContextMenu.h"
#include "Element/UIChatBar.h"
#include "Element/UIMenu.h"
#include "Element/UIMiniMap.h"
#include "Element/UIStatusBar.h"
#include "Element/UIPartyPanel.h"
#include "Element/UIPartyWindow.h"
#include "Element/Buff/UIBuffList.h"
#include "Element/Inventory/UIInventory.h"
#include "Element/Inventory/UIItemTooltip.h"
#include "Subsystems/PartySubsystem.h"
#include "Subsystems/PlayerSubsystem.h"

UIInGameState::UIInGameState() :
    context_menu_(nullptr),
    party_panel_(nullptr),
    party_window_(nullptr)
{
    mini_map_ = AddElement<UIMiniMap>(UIMiniMap::StaticClass(), L"MiniMap");
    
    char_bar_ = AddElement<UIChatBar>(UIChatBar::StaticClass(), L"ChatBar");
    char_bar_->SetAbsolutePosition({0.f, 684.f});
    
    inventory_ = AddElement<UIInventory>(UIInventory::StaticClass(), L"Inventory");
    inventory_->SetActive(false);
    
    AddElement<UIStatusBar>(UIStatusBar::StaticClass(), L"StatusBar");
    
    party_panel_ = AddElement<UIPartyPanel>(UIPartyPanel::StaticClass(), L"PartyPanel");
    party_panel_->SetAbsolutePosition({EngineSettings::Get()->GetScreenWidth() - party_panel_->GetSize().x - 25, 25});
    party_panel_->SetActive(false);

    party_window_ = AddElement<UIPartyWindow>(UIPartyWindow::StaticClass(), L"PartyWindow");
    party_window_->SetAbsolutePosition({100.f, 100.f});
    party_window_->SetActive(false);

    item_tooltip_ = AddElement<UIItemTooltip>(UIItemTooltip::StaticClass(), L"ItemTooltip");
    item_tooltip_->SetAbsolutePosition({ 100.f, 100.f });
    item_tooltip_->SetSize({ 322.f, 122.f });
    item_tooltip_->SetActive(false);
    item_tooltip_->SetIgnoreRayCast(true);

    menu_ = AddElement<UIMenu>(UIMenu::StaticClass(), L"Menu");
    menu_->SetActive(false);

    context_menu_ = AddElement<UIContextMenu>(UIContextMenu::StaticClass(), L"ContextMenu");
    context_menu_->SetActive(false);

    AddElement<UIBuffList>(UIBuffList::StaticClass(), L"BuffList");
}

void UIInGameState::Init()
{
    UIState::Init();
    PartySubsystem::Get()->RedrawUI();
}

bool UIInGameState::OnKey(uint32_t scancode, bool is_pressed)
{
    bool is_handled = UIState::OnKey(scancode, is_pressed);
    
    if (!is_handled && is_pressed)
    {
        if (scancode == static_cast<uint32_t>(Scancode::kKeyI) && !IsEditingText())
        {
            inventory_->SetActive(!inventory_->IsActive());
            is_handled = true;
        }
        
        if (scancode == static_cast<uint32_t>(Scancode::kKeyEnter) && !IsEditingText())
        {
            char_bar_->FocusInput();
            is_handled = true;
        }

        if (scancode == static_cast<uint32_t>(Scancode::kKeyEscape) && !IsEditingText())
        {
            menu_->SetActive(!menu_->IsActive());
            is_handled = true;
        }

        if (scancode == static_cast<uint32_t>(Scancode::kKeyP) && !IsEditingText())
        {
            party_window_->SetActive(!party_window_->IsActive());
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
