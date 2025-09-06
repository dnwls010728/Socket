#pragma once
#include "IPacketHandler.h"

class SkillCastHandler : public IPacketHandler
{
public:
    virtual ~SkillCastHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
