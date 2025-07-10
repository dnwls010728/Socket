#pragma once
#include "IPacketHandler.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

class PlayerStatsUpdateHandler : public IPacketHandler
{
public:
    virtual ~PlayerStatsUpdateHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;
    
};
