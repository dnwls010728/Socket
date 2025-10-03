#include "pch.h"
#include "SkillUpdateHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"

bool SkillUpdateHandler::Handle(Net::IPacket* packet)
{
    SkillUpdatePacket* response = dynamic_cast<SkillUpdatePacket*>(packet);
    if (!response) return false;

    PlayerSubsystem* player = PlayerSubsystem::Get();
    player->UpdateSkill(response->skill_id, response->skill_level);
    return true;
}
