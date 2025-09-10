#pragma once
#include "IPacketHandler.h"

class PartyInfoChangedHandler : public IPacketHandler
{
public:
    virtual ~PartyInfoChangedHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
