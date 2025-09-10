#include "pch.h"
#include "PartyJoinHandler.h"

#include <CustomPacket.h>

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/PartySubsystem.h"

bool PartyJoinHandler::Handle(Net::IPacket* packet)
{
    PartyJoinPacket* join_packet = dynamic_cast<PartyJoinPacket*>(packet);

    auto subsystem = PartySubsystem::Get();
    subsystem->Join(join_packet->party_id, join_packet->party_name);
    subsystem->SetHostMemberID(join_packet->host_id);

    for (auto member : join_packet->members)
    {
        subsystem->AddOrUpdateMember(member);
    }
    
    return true;
}
