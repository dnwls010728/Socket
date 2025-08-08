#include "pch.h"
#include "UIInGameState.h"

#include "Ui/UIPopup.h"
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
}

void UIInGameState::Init()
{
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

        // TEST
        if (key_code == 'F' && !IsEditingText())
        {
            UIPopup::PopupParam param;
            param.caption = L"테스트 입니다. 아무말이나 입력하세요";
            param.option = UIPopup::PopupOption::OK | UIPopup::PopupOption::Cancel | UIPopup::PopupOption::Edit;
            param.content_type = UIEditableText::ContentType::kPassword;
            param.callback = [&](const std::wstring& text,  UIPopup::PopupOption option)
            {
                if (option == UIPopup::PopupOption::OK)
                {
                    
                    UIPopup::PopupParam param;
                    param.caption = text;
                    param.placeholder = text;
                    param.option = UIPopup::PopupOption::OK | UIPopup::PopupOption::Cancel | UIPopup::PopupOption::No | UIPopup::PopupOption::Yes;
                    param.callback = [&](const std::wstring& text,  UIPopup::PopupOption option) { return true;};
                    UIPopup::ShowPopup(param);
                    return false;
                }
                return true;
            };
            UIPopup::ShowPopup(param);
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
