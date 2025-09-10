#include "pch.h"
#include "PartyInfoChangedHandler.h"

#include <CustomPacket.h>

#include "Actors/NetworkActor.h"
#include "Subsystems/NetworkSubsystem.h"
#include <sstream>

#include "Subsystems/PartySubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/Element/UIPopup.h"

bool PartyInfoChangedHandler::Handle(Net::IPacket* packet)
{
    PartyInfoChangedPacket* received_packet = dynamic_cast<PartyInfoChangedPacket*>(packet);
    if (!received_packet) return false;

    try
    {
        switch (received_packet->type)
        {
        case PartyInfoType::kHostID:
            PartySubsystem::Get()->SetHostMemberID(std::stoi(received_packet->value));
            break;
        case PartyInfoType::kPartyName:
            PartySubsystem::Get()->SetPartyName(received_packet->value);
            break;
        default:
            break;
        }
    }
    catch (...)
    {
    }

    PartySubsystem::Get()->RedrawUI();
    
    return true;
}
