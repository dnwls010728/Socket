#pragma once
#include "IPacketHandler.h"

class CheckNameHandler : public IPacketHandler
{
public:
    virtual ~CheckNameHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
