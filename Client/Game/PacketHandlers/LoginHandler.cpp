#include "pch.h"
#include "LoginHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UILoginState.h"
#include "UI/Element/UIPopup.h"
#include "UI/UIState.h"
#include "UI/Element/CharacterSelect/UICharacterSelect.h"
#include "UI/Element/UILogin.h"

bool LoginHandler::Handle(Net::IPacket* packet)
{
    LoginResponse* received_packet = dynamic_cast<LoginResponse*>(packet);
    if (!received_packet) return false;

    UILoginState* state = dynamic_cast<UILoginState*>(UI::Get()->GetState());
    if (state)
    {
        if (auto* element = state->FindElement<UILogin>(L"Login"))
            element->SetLoginDisabled(false);
    }
    
    if (received_packet->is_success)
    {
        SessionSubsystem::Get()->SetState(SessionState::kLoggedIn);
        PlayerSubsystem::Get()->profiles_ = received_packet->profiles;
        
        if (state)
        {
            if (auto* element = state->FindElement<UILogin>(L"Login"))
                state->RemoveElement(element);
            
            state->AddElement<UICharacterSelect>(UICharacterSelect::StaticClass(), L"CharacterSelect");
        }
    }
    else
    {
        UIPopup::PopupParam param;
        param.caption = received_packet->message;
        param.option = UIPopup::PopupOption::OK;
        param.callback = [&](const std::wstring& text,  UIPopup::PopupOption option){ return true;};
        UIPopup::ShowPopup(param);
    }

    return true;
}
