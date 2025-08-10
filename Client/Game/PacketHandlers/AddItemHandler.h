#pragma once
#include "IPacketHandler.h"

class AddItemHandler : public IPacketHandler
{
public:
    virtual ~AddItemHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
