#pragma once
#include "IPacketHandler.h"

class DeleteCharacterHandler : public IPacketHandler
{
public:
    virtual ~DeleteCharacterHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
