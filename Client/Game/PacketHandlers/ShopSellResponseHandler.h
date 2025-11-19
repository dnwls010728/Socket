#pragma once
#include "IPacketHandler.h"

class ShopSellResponseHandler : public IPacketHandler
{
public:
    virtual ~ShopSellResponseHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
};
