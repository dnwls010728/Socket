#pragma once
#include "IPacketHandler.h"

class ObjectSpawnHandler : public IPacketHandler
{
public:
    virtual ~ObjectSpawnHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
