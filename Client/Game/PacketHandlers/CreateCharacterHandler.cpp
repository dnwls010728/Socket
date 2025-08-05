#include "pch.h"
#include "CreateCharacterHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "UI/UI.h"
#include "UI/UIState.h"
#include "UI/Element/UICharacterCreate.h"
#include "UI/Element/CharacterSelect/UICharacterSelect.h"

bool CreateCharacterHandler::Handle(Net::IPacket* packet)
{
    CreateCharacterResponse* received_packet = dynamic_cast<CreateCharacterResponse*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem::Get()->AddProfile(received_packet->profile);
    
    if (auto* state = UI::Get()->GetState())
    {
        if (auto* element = state->FindElement<UICharacterCreate>(L"CharacterCreate"))
            state->RemoveElement(element);
        
        state->AddElement<UICharacterSelect>(UICharacterSelect::StaticClass(), L"CharacterSelect");
    }

    return true;
}
