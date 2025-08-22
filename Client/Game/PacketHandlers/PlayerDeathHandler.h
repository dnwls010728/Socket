#pragma once
#include "IPacketHandler.h"

class PlayerDeathHandler : public IPacketHandler
{
public:
    virtual ~PlayerDeathHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
