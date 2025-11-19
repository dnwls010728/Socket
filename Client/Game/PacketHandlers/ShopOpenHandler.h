#pragma once
#include "IPacketHandler.h"

class ShopOpenHandler : public IPacketHandler
{
public:
    virtual ~ShopOpenHandler() override = default;
    
    virtual bool Handle(Net::IPacket* packet) override;
    
};
