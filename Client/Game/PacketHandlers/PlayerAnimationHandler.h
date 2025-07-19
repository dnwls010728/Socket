#pragma once
#include "IPacketHandler.h"

class PlayerAnimationHandler : public IPacketHandler
{
public:
    virtual ~PlayerAnimationHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
