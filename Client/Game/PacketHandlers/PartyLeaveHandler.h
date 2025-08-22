#pragma once
#include "IPacketHandler.h"

class PartyLeaveHandler : public IPacketHandler
{
public:
    virtual ~PartyLeaveHandler() override = default;
    virtual bool Handle(Net::IPacket* packet) override;
};
