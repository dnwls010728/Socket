#pragma once
#include "IPacketHandler.h"
#include "Actors/Characters/Player/PlayerCharacter.h"

class PlayerStatsUpdateHandler : public IPacketHandler
{
public:
    virtual ~PlayerStatsUpdateHandler() override = default;

    virtual bool Handle(Net::IPacket* packet) override;

    bool IsValid(const std::array<uint32_t, static_cast<uint8_t>(PlayerStat::kCount)>& stats, PlayerStat stat) const;
    
};
