#include "pch.h"
#include "ServerPacketHandler.h"
#include <functional>
#include <iostream>

#include "SocketEventManager.h"
PacketHandlerFunc GPacketHandler[UINT16_MAX];

void HandleInvalid(BYTE* buf, int32_t len)
{
    std::cout << "Invalid Packet" << std::endl;    
}
