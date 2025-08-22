#pragma once
#include "IPacketHandler.h"

class PopupHandler : public IPacketHandler
{
public:
    virtual ~PopupHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
