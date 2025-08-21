#include "pch.h"
#include "PlayerStatsUpdateHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool PlayerStatsUpdateHandler::Handle(Net::IPacket* packet)
{
    PlayerStatsUpdatePacket* received_packet = dynamic_cast<PlayerStatsUpdatePacket*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();

    PlayerStat mask = received_packet->mask;
    if (EnumHasAnyFlags(mask, PlayerStat::kHP))
        player_subsystem->UpdateStat(PlayerStat::kHP, received_packet->hp);
    
    if (EnumHasAnyFlags(mask, PlayerStat::kMaxHP))
        player_subsystem->UpdateStat(PlayerStat::kMaxHP, received_packet->max_hp);

    if (EnumHasAnyFlags(mask, PlayerStat::kExp))
        player_subsystem->UpdateStat(PlayerStat::kExp, received_packet->exp);

    if (EnumHasAnyFlags(mask, PlayerStat::kLv))
        player_subsystem->UpdateStat(PlayerStat::kLv, received_packet->lv);
    
    return true;
}
