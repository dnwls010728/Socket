#include "pch.h"
#include "PartyJoinHandler.h"

#include <CustomPacket.h>

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Subsystems/NetworkSubsystem.h"

bool PartyJoinHandler::Handle(Net::IPacket* packet)
{
    PartyJoinPacket* join_packet = dynamic_cast<PartyJoinPacket*>(packet);

    auto player = NetworkSubsystem::Get()->GetPlayer();
    if (!player)
    {
        return true;
    }

    player->SetPartyID(join_packet->party_id);
    
    return true;
}
