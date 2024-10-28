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

void HandleEnter(S_EnterPacket& pkt)
{
    std::cout << "Enter Packet" << std::endl;
    std::shared_ptr<S_EnterPacket> pktRef = std::make_shared<S_EnterPacket>(pkt);
    //얘가 로그인 성공을 알리는 response
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    socket_event_manager->RegisterEvent(pktRef,S_PKT_ENTER);
}

void HandleMoving(S_MovingPacket& pkt)
{
    std::cout<< "Moving" << std::endl;
    
}

void HandleBroadcastEnter(S_BroadcastingEnterPacket& pkt)
{
    std::cout << "Broadcast Packet" << std::endl;
    std::shared_ptr<S_BroadcastingEnterPacket> pktRef = std::make_shared<S_BroadcastingEnterPacket>(pkt);
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    socket_event_manager->RegisterEvent(pktRef,S_PKT_BROADCASTING_ENTER);
}
