#pragma once
#include "IPacketHandler.h"

class ObjectPositionHandler : public IPacketHandler
{
public:
    virtual ~ObjectPositionHandler() override = default;
    
    virtual bool Handle(Net::IPacket* packet) override;
};
