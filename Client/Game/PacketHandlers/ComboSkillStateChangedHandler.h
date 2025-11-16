#pragma once
#include "IPacketHandler.h"

class ComboSkillStateChangedHandler : public IPacketHandler
{
public:
    virtual ~ComboSkillStateChangedHandler() override = default;
    virtual bool Handle(Net::IPacket* packet) override;
};
