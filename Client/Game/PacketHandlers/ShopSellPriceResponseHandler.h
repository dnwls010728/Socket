#pragma once
#include "IPacketHandler.h"

class ShopSellPriceResponseHandler : public IPacketHandler
{
public:
    virtual ~ShopSellPriceResponseHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
};
