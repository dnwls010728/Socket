#pragma once
#include "IPacketHandler.h"

class SelectCharacterHandler : public IPacketHandler
{
public:
    virtual ~SelectCharacterHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
