#include "pch.h"
#include "ServerPacketHandler.h"
#include <functional>
#include <iostream>
PacketHandlerFunc GPacketHandler[UINT16_MAX];
void HandleInvalid(BYTE* buf, int32_t len)
{
    std::cout << "Invalid Packet" << std::endl;    
}

void HandleEnter(S_EnterPacket& pkt)
{
    std::cout << "Enter Packet" << std::endl;
}
