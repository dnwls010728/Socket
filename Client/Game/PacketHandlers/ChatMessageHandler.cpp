#include "pch.h"
#include "ChatMessageHandler.h"

#include <CustomPacket.h>
#include <ranges>

#include "Actors/NetworkActor.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Subsystems/NetworkSubsystem.h"
#include "UI/UI.h"
#include "UI/UIState.h"
#include "UI/Element/UIChatBar.h"

bool ChatMessageHandler::Handle(Net::IPacket* packet)
{
    ChatMessagePacket* received_packet = dynamic_cast<ChatMessagePacket*>(packet);
    if (!received_packet) return false;

    for (const auto& network_actor : NetworkSubsystem::Get()->GetNetworkActors() | std::views::values)
    {
        if (network_actor->GetObjectID() == received_packet->unique_id)
        {
            std::shared_ptr<PlayerCharacter> player_character = std::dynamic_pointer_cast<PlayerCharacter>(network_actor);
            if (IsValid(player_character))
            {
                std::wstring character_name = player_character->GetCharacterName();
                std::wstring message = received_packet->message;
                player_character->Speak(character_name + L": " + message);
                
                if (auto state = UI::Get()->GetState())
                {
                    if (auto element = state->FindElement<UIChatBar>(L"ChatBar"))
                        element->AddMessage(player_character->GetCharacterName() + L": " + message);
                }
            }

            break;
        }
    }
    
    return true;
}
