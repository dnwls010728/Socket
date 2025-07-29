#pragma once
#include "IPacketHandler.h"

class LoginHandler : public IPacketHandler
{
public:
    virtual ~LoginHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
