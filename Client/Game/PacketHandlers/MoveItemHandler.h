#pragma once
#include "IPacketHandler.h"

class MoveItemHandler : public IPacketHandler
{
public:
    virtual ~MoveItemHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
