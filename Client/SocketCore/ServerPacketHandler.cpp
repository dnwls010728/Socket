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

void HandleMoving(S_MovingPacket& pkt)
{
    std::cout<< "Moving" << std::endl;
    
}

void HandleBroadcastEnter(S_BroadcastingEnterPacket& pkt)
{
    std::cout << "Broadcast Packet" << std::endl;
    
}
