#pragma once
#include "IPacketHandler.h"

class ObjectAnimationHandler : public IPacketHandler
{
public:
    virtual ~ObjectAnimationHandler() override = default;
    
    virtual bool Handle(Net::IPacket* packet) override;
};
