#include "pch.h"
#include "DeleteCharacterHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "UI/UI.h"
#include "UI/UIState.h"
#include "UI/Element/CharacterSelect/UICharacterSelect.h"

bool DeleteCharacterHandler::Handle(Net::IPacket* packet)
{
    DeleteCharacterResponse* received_packet = dynamic_cast<DeleteCharacterResponse*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem* subsystem = PlayerSubsystem::Get();
    subsystem->DeleteProfile(received_packet->character_id);

    if (auto* state = UI::Get()->GetState())
    {
        if (auto* element = state->FindElement<UICharacterSelect>(L"CharacterSelect"))
            element->RefreshSlots(subsystem->GetProfiles());
    }

    return true;
}
