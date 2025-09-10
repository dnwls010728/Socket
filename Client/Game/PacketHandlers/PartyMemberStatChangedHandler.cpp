#include "pch.h"
#include "PartyMemberStatChangedHandler.h"

#include <CustomPacket.h>
#include "Subsystems/PartySubsystem.h"

bool PartyMemberStatChangedHandler::Handle(Net::IPacket* packet)
{
    auto* received_packet = dynamic_cast<PartyMemberStatChangedPacket*>(packet);
    if (!received_packet) return false;

    PartySubsystem::Get()->UpdateMemberStat(received_packet->member_id, received_packet->stat, received_packet->value);
    return true;
}
