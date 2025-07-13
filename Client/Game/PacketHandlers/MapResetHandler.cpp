#include "pch.h"
#include "MapResetHandler.h"

#include <CustomPacket.h>

bool MapResetHandler::Handle(Net::IPacket* packet)
{
    MapResetPacket* received_packet = dynamic_cast<MapResetPacket*>(packet);
    if (!received_packet) return false;

    World::Get()->OpenLevel(L"Game");
    return true;
}
