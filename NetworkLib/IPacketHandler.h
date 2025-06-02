#pragma once
#include "IPacket.h"

class IPacketHandler
{
public:
    virtual ~IPacketHandler() = default;

    virtual bool Handle(Net::IPacket* packet) = 0;

protected:
    IPacketHandler() = default;
    
};
