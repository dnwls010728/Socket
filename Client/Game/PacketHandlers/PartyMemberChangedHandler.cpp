#include "pch.h"
#include "PartyMemberChangedHandler.h"

#include <CustomPacket.h>
#include "Subsystems/PartySubsystem.h"

bool PartyMemberChangedHandler::Handle(Net::IPacket* packet)
{
    auto* received_packet = dynamic_cast<PartyMemberChangedPacket*>(packet);
    if (!received_packet) return false;

    PartySubsystem* subsystem = PartySubsystem::Get();
    switch (received_packet->change)
    {
    case PartyMemberChangeType::kJoin:
    case PartyMemberChangeType::kUpdate:
        subsystem->AddOrUpdateMember(received_packet->member);
        break;
    case PartyMemberChangeType::kLeave:
        subsystem->DeleteMember(received_packet->member.player_id);
        break;
    }
    return true;
}
