#pragma once
#include "IPacketHandler.h"

class DestroyObjectHandler : public IPacketHandler
{
public:
    virtual ~DestroyObjectHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
