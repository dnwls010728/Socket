#pragma once
#include "IPacketHandler.h"

class MapResetHandler : public IPacketHandler
{
public:
    virtual ~MapResetHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
