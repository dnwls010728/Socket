#pragma once
#include "IPacketHandler.h"

class TakeDamageHandler : public IPacketHandler
{
public:
    virtual ~TakeDamageHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
