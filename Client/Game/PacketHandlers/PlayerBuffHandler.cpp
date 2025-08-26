#include "pch.h"
#include "PlayerBuffHandler.h"

#include <CustomPacket.h>

bool PlayerBuffHandler::Handle(Net::IPacket* packet)
{
    PlayerBuffPacket* received_packet = dynamic_cast<PlayerBuffPacket*>(packet);
    if (!received_packet) return false;

    Logger::Print(L"Time: %f", received_packet->server_time + received_packet->duration);

    return true;
}
