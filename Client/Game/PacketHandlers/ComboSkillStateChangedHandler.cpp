#include "pch.h"
#include "ComboSkillStateChangedHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"

bool ComboSkillStateChangedHandler::Handle(Net::IPacket* packet)
{
    auto* received_packet = dynamic_cast<ComboSkillStateChangedPacket*>(packet);
    if (!received_packet) return false;

    PlayerSubsystem* player = PlayerSubsystem::Get();
    player->GetSkillManager()->SetSkillComboIndex(received_packet->skill_id, received_packet->combo_idx);
    player->GetSkillManager()->UseSkill(received_packet->skill_id, received_packet->cooldown_expired_time);
    
    SkillListUpdatedData data;
    data.skills = player->GetSkillManager()->GetSkillList();
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kSkillsUpdated, data);
    return true;
}
