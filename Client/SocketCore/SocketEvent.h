#pragma once
#include "ServerPacketHandler.h"
struct EnterEvent
{
    
    uint32_t userId;
    std::string name;
};

struct MovingEvent
{
    
    uint32_t userId;
    int32_t locationX;
    int32_t locationY;
};

struct BroadcastingEnterEvent
{
    
    uint32_t userId;
    std::string name;
};

struct SocketEvent
{
    Type::uint32 type;
    EnterEvent enter;
    MovingEvent moving;
    BroadcastingEnterEvent broadcastingEnter;
};


