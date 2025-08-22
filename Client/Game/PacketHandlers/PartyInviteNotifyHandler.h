#pragma once
#include "IPacketHandler.h"

class PartyInviteNotifyHandler : public IPacketHandler
{
public:
    virtual ~PartyInviteNotifyHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
