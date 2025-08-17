#pragma once
#include "IPacketHandler.h"

class PartyJoinHandler : public IPacketHandler
{
public:
    virtual ~PartyJoinHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
