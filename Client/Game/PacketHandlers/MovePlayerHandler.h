#pragma once
#include "IPacketHandler.h"

class MovePlayerHandler : public IPacketHandler
{
public:
    virtual ~MovePlayerHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
