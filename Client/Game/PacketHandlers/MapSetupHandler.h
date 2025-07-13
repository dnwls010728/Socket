#pragma once
#include "IPacketHandler.h"

class MapSetupHandler : public IPacketHandler
{
public:
    virtual ~MapSetupHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
