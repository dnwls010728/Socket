#include "pch.h"
#include "SocketEventManager.h"

SocketEventManager::SocketEventManager()
{
    
}

bool SocketEventManager::PollEvent(SocketEvent& socketEvent)
{
    if(eventQueue.empty())return false;

    socketEvent = eventQueue.front();
    eventQueue.pop();

    return true;
}

bool SocketEventManager::RegisterEvent(Packet pkt,uint16_t pktId)
{
    if(pktId == S_PKT_ENTER)
    {
        S_EnterPacket* enterPkt = (S_EnterPacket*)&pkt;
        EnterEvent event{};
        event.userId = enterPkt->_userId;
        SocketEvent socketEvent{};
        socketEvent.type = S_PKT_ENTER;
        socketEvent.enter = event;
        eventQueue.push(socketEvent);
        return true;
    }
    else if(pktId == S_PKT_MOVING)
    {
        S_MovingPacket* movingPkt = (S_MovingPacket*)&pkt;
        MovingEvent event{};
        event.userId = movingPkt->_userId;
        event.locationX = movingPkt->_locationX;
        event.locationY = movingPkt->_locationY;

        SocketEvent socketEvent{};
        socketEvent.moving = event;
        socketEvent.type = S_PKT_MOVING;
        eventQueue.push(socketEvent);
        return true;
    }else if(pktId == S_PKT_BROADCASTING_ENTER)
    {
        S_BroadcastingEnterPacket* broadcastingEnterPkt = (S_BroadcastingEnterPacket*)&pkt;
        BroadcastingEnterEvent event{};
        event.userId = broadcastingEnterPkt->_userId;
        event.name = broadcastingEnterPkt->_name;
        SocketEvent socketEvent{};
        socketEvent.broadcastingEnter=event;
        socketEvent.type = S_PKT_BROADCASTING_ENTER;
        eventQueue.push(socketEvent);
        return true;
    }
    return false;
}

void SocketEventManager::Clear()
{
}
