#pragma once
#include "IPacketHandler.h"

class PartyMemberChangedHandler : public IPacketHandler
{
public:
    virtual ~PartyMemberChangedHandler() override = default;
    virtual bool Handle(Net::IPacket* packet) override;
};
