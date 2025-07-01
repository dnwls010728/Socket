#pragma once
#include "IPacketHandler.h"

class SpawnPlayerHandler : public IPacketHandler
{
public:
    virtual ~SpawnPlayerHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
