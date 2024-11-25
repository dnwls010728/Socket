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

void HandleEnter(std::shared_ptr<S_EnterPacket> pkt)
{
    std::cout << "Enter Packet" << std::endl;
    //std::shared_ptr<S_EnterPacket> pktRef = std::make_shared<S_EnterPacket>(pkt);
    //얘가 로그인 성공을 알리는 response
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    socket_event_manager->RegisterEvent(pkt,S_PKT_ENTER);
}

void HandleMoving(std::shared_ptr<S_MovingPacket> pkt)
{
    std::cout<< "Moving" << std::endl;
    //std::shared_ptr<S_MovingPacket> pktRef = std::make_shared<S_MovingPacket>(pkt);
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    socket_event_manager->RegisterEvent(pkt,S_PKT_MOVING);
    
}

void HandleBroadcastEnter(std::shared_ptr<S_BroadcastingEnterPacket> pkt)
{
    std::cout << "Broadcast Packet" << std::endl;
    //std::shared_ptr<S_BroadcastingEnterPacket> pktRef = std::make_shared<S_BroadcastingEnterPacket>(pkt);
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    socket_event_manager->RegisterEvent(pkt,S_PKT_BROADCASTING_ENTER);
}

void HandleEnterOtherUser(std::shared_ptr<S_EnterOtherUserPacket> pkt)
{
    std:: cout << "Enter Other User Packet" << std::endl;
    for(uint8_t i=0;i<pkt->currentUserCnt_;i++)
    {
        std::cout<< pkt->nameArr_[i]<<std::endl;
    }
    //std::shared_ptr<S_EnterOtherUserPacket> pktRef = std::make_shared<S_EnterOtherUserPacket>(pkt);
    SocketEventManager* socket_event_manager = SocketEventManager::Get();
    socket_event_manager->RegisterEvent(pkt,S_PKT_ENTER_OTHER_USER);
}


