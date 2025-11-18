#pragma once
#include "IPacketHandler.h"

class ShopBuyResponseHandler : public IPacketHandler
{
public:
    virtual ~ShopBuyResponseHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
};
