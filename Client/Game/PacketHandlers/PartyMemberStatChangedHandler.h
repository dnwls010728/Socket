#pragma once
#include "IPacketHandler.h"

class PartyMemberStatChangedHandler : public IPacketHandler
{
public:
    virtual ~PartyMemberStatChangedHandler() override = default;
    virtual bool Handle(Net::IPacket* packet) override;
};
