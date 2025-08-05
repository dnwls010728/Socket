#include "pch.h"
#include "DeleteCharacterHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool DeleteCharacterHandler::Handle(Net::IPacket* packet)
{
    DeleteCharacterResponse* received_packet = dynamic_cast<DeleteCharacterResponse*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem::Get()->DeleteProfile(received_packet->character_id);

    return true;
}
