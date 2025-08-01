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

void UIInGameState::Tick(float deltaTime)
{
    UIState::Tick(deltaTime);

    while (!pending_tasks_.empty())
    {
        pending_tasks_.front()();
        pending_tasks_.pop();
    }
}

void UIInGameState::ShowPopup(std::wstring caption, PopupOption option, std::function<bool(std::wstring, PopupOption)> callback)
{
    UIPopup *popup = AddElement<UIPopup>(UIPopup::StaticClass(), L"Popup");
    popup->SetPopup(caption, option);
    popup->SetCallback([&, callback, popup](std::wstring input_text, PopupOption option)
    {
        if (callback && callback(input_text, option))
        {
            PostTask([this, popup]()
            {
                popup->SetActive(false);
                RemoveElement(popup);
            });
        }
    });
    Math::Vector2 pos{
        static_cast<float>(EngineSettings::Get()->GetScreenWidth()) / 2 - popup->GetSize().x / 2,
        static_cast<float>(EngineSettings::Get()->GetScreenHeight()) / 2 - popup->GetSize().y / 2,
    };
    popup->SetAbsolutePosition(pos);
    popup->SetActive(true);
    
    if ((option & PopupOption::Edit)  == PopupOption::Edit)
        popup->SetFoucsInputText();
    else
        UI::Get()->SetFocus(popup);
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
            ShowPopup(L"테스트 입니다. 아무말이나 입력하세요", PopupOption::OK | PopupOption::Cancel | PopupOption::Edit, [&](std::wstring input_text, PopupOption option)->bool
            {
                if (option == PopupOption::OK)
                {
                    ShowPopup(input_text.c_str(), PopupOption::No | PopupOption::OK | PopupOption::Cancel| PopupOption::Yes, [&](std::wstring input_text, PopupOption option)->bool
                    {
                        return true;
                    });
                    return false;
                }
                return true;
            });
        }

        if (key_code == VK_RETURN && !IsEditingText())
        {
            char_bar_->FocusInput();
            is_handled = true;
        }
    }
    
    return is_handled;
}

void UIInGameState::PostTask(std::function<void()> task)
{
    pending_tasks_.push(std::move(task));
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
