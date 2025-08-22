#include "pch.h"
#include "PartyLeaveHandler.h"

#include <CustomPacket.h>
#include "Subsystems/PartySubsystem.h"

bool PartyLeaveHandler::Handle(Net::IPacket* packet)
{
    PartySubsystem::Get()->Clear();
    return true;
}
