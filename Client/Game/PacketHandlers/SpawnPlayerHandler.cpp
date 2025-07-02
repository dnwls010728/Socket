#include "pch.h"
#include "SpawnPlayerHandler.h"

#include <CustomPacket.h>

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Subsystems/NetworkSubsystem.h"

bool SpawnPlayerHandler::Handle(Net::IPacket* packet)
{
    SpawnPlayerPacket* received_packet = dynamic_cast<SpawnPlayerPacket*>(packet);
    if (!received_packet) return false;

    NetworkSubsystem* network_subsystem = NetworkSubsystem::Get();

    std::shared_ptr<PlayerCharacter> player_character = network_subsystem->SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), received_packet->character_id);
    if (IsValid(player_character))
    {
        float position_x = received_packet->position_x;
        float position_y = received_packet->position_y;

        player_character->InitSpawn(received_packet->name, {position_x, position_y});
        network_subsystem->other_players_.emplace_back(player_character);
    }

    return true;
}
