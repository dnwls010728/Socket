#pragma once
#include "IPacketHandler.h"

class DoSelectCardHandler : public IPacketHandler
{
public:
    virtual ~DoSelectCardHandler() override = default;
    virtual bool Handle(Net::IPacket* packet) override;
};
