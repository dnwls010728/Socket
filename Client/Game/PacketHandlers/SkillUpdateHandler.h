#pragma once
#include "IPacketHandler.h"

class SkillUpdateHandler : public IPacketHandler
{
public:
    virtual ~SkillUpdateHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
