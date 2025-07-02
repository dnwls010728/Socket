#pragma once
#include "IPacketHandler.h"

class ChangeMapHandler : public IPacketHandler
{
public:
    virtual ~ChangeMapHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
