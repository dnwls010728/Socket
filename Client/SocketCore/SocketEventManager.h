#pragma once
#include <queue>

#include "Singleton.h"
#include "SocketEvent.h"

class SocketEventManager : public Singleton<SocketEventManager>
{
public:
    SocketEventManager();
    virtual ~SocketEventManager() override = default;

    bool PollEvent(SocketEvent& socketEvent);
    bool RegisterEvent(Packet pkt,uint16_t pktId);

private:
    friend class Core;

    void Clear();
    
    std::queue<SocketEvent> eventQueue;
    
};
