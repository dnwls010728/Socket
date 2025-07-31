#include "pch.h"
#include "LoginHandler.h"

#include <CustomPacket.h>

#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UILoginState.h"
#include "UI/UIState.h"
#include "UI/Element/CharacterSelect/UICharacterSelect.h"
#include "UI/Element/UILogin.h"

bool LoginHandler::Handle(Net::IPacket* packet)
{
    LoginResponse* received_packet = dynamic_cast<LoginResponse*>(packet);
    if (!received_packet) return false;

    UILoginState* state = dynamic_cast<UILoginState*>(UI::Get()->GetState());
    if (state) state->GetLogin()->SetLoginDisabled(false);
    
    if (received_packet->is_success)
    {
        SessionSubsystem::Get()->SetState(SessionState::kLoggedIn);
        
        if (state)
        {
            state->InitModel(received_packet->characters);
            
            state->GetLogin()->SetActive(false);
            state->GetCharacterSelect()->SetActive(true);
        }
    }
    else
    {
    }

    return true;
}
