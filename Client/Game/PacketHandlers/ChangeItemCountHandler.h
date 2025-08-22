#pragma once
#include "IPacketHandler.h"

class ChangeItemCountHandler : public IPacketHandler
{
public:
    virtual ~ChangeItemCountHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
