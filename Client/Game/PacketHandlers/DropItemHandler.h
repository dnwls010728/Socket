#pragma once
#include "IPacketHandler.h"

class DropItemHandler : public IPacketHandler
{
public:
    virtual ~DropItemHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
