#include "pch.h"
#include "LoginHandler.h"

#include <CustomPacket.h>

#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UIState.h"
#include "UI/Element/UILogin.h"

bool LoginHandler::Handle(Net::IPacket* packet)
{
    LoginResponse* received_packet = dynamic_cast<LoginResponse*>(packet);
    if (!received_packet) return false;

    UIState* state = UI::Get()->GetState();
    
    if (received_packet->is_success)
    {
        SessionSubsystem::Get()->SetState(SessionState::kLoggedIn);
        
        if (state)
        {
            if (auto element = state->FindElement<UILogin>(L"Login"))
                element->SetActive(false);
        }
    }
    else
    {
        if (state)
        {
            if (auto element = state->FindElement<UILogin>(L"Login"))
                element->SetLoginDisabled(false);
        }
    }

    return true;
}
