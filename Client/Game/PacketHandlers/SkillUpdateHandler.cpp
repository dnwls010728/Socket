#include "pch.h"
#include "SkillUpdateHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"

bool SkillUpdateHandler::Handle(Net::IPacket* packet)
{
    SkillUpdatePacket* response = dynamic_cast<SkillUpdatePacket*>(packet);
    if (!response) return false;

    PlayerSubsystem* player = PlayerSubsystem::Get();
    player->GetSkillManager()->AddOrUpdateSkill(response->skill.skill_id, response->skill.level, response->skill.cooldown);
    
    SkillListUpdatedData data;
    data.skills = player->GetSkillManager()->GetSkillList();
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kSkillsUpdated, data);

    return true;
}
