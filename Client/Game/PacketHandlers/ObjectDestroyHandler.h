#pragma once
#include "IPacketHandler.h"

class ObjectDestroyHandler : public IPacketHandler
{
public:
    virtual ~ObjectDestroyHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
