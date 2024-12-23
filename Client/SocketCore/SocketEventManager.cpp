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

bool SocketEventManager::RegisterEvent(const std::shared_ptr<Packet>& pkt,uint16_t pktId)
{
    
    return false;
}

void SocketEventManager::Clear()
{
}
