#include "pch.h"
#include "PlayerStatsUpdateHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool PlayerStatsUpdateHandler::Handle(Net::IPacket* packet)
{
    PlayerStatsUpdatePacket* received_packet = dynamic_cast<PlayerStatsUpdatePacket*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();
    for (uint32_t i = 0; i < 4; ++i)
    {
        bool has_flag = (received_packet->flags & (1 << i)) != 0;
        if (!has_flag) continue;
        
        PlayerStat stat = static_cast<PlayerStat>(1 << i);
        uint32_t value = received_packet->stats[i];
        
        player_subsystem->UpdateStat(stat, value);
    }
    
    return true;
}
