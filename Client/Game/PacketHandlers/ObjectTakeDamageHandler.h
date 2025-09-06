#pragma once
#include "IPacketHandler.h"

class ObjectTakeDamageHandler : public IPacketHandler
{
public:
    virtual ~ObjectTakeDamageHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
