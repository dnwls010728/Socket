#pragma once
#include "IPacketHandler.h"

class MapLoadHandler : public IPacketHandler
{
public:
    virtual ~MapLoadHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
