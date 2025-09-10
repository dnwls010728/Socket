#pragma once
#include "IPacketHandler.h"

class PlacementStartHandler : public IPacketHandler
{
public:
    virtual ~PlacementStartHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
