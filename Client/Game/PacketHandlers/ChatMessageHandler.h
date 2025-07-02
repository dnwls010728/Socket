#pragma once
#include "IPacketHandler.h"

class ChatMessageHandler : public IPacketHandler
{
public:
    virtual ~ChatMessageHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
