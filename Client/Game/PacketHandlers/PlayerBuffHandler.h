#pragma once
#include "IPacketHandler.h"

class PlayerBuffHandler : public IPacketHandler
{
public:
    virtual ~PlayerBuffHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
