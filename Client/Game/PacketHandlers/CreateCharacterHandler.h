#pragma once
#include "IPacketHandler.h"

class CreateCharacterHandler : public IPacketHandler
{
public:
    virtual ~CreateCharacterHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
