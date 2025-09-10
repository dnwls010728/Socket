#pragma once
#include "IPacketHandler.h"

class PlacementStopHandler : public IPacketHandler
{
public:
    virtual ~PlacementStopHandler() override = default;
    
    virtual bool Handle(Net::IPacket* packet) override;
    
};
