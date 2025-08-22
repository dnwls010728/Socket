#pragma once
#include "IPacketHandler.h"

class ColorGainHandler : public IPacketHandler
{
public:
    virtual ~ColorGainHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
