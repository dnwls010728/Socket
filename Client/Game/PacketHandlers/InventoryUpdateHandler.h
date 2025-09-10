#pragma once
#include "IPacketHandler.h"

class InventoryUpdateHandler : public IPacketHandler
{
public:
    virtual ~InventoryUpdateHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
