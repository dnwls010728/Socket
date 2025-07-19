#pragma once
#include "IPacketHandler.h"

class SpawnObjectHandler : public IPacketHandler
{
public:
    virtual ~SpawnObjectHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
