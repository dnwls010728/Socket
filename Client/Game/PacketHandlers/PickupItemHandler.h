#pragma once
#include "IPacketHandler.h"

class PickupItemHandler : public IPacketHandler
{
public:
    virtual ~PickupItemHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
