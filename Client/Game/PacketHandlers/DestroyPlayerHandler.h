#pragma once
#include "IPacketHandler.h"

class DestroyPlayerHandler : public IPacketHandler
{
public:
    virtual ~DestroyPlayerHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
