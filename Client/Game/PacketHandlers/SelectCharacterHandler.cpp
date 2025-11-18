#include "pch.h"
#include "SelectCharacterHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "Subsystems/InputActions/InputActions.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"

bool SelectCharacterHandler::Handle(Net::IPacket* packet)
{
    SelectCharacterResponse* response = dynamic_cast<SelectCharacterResponse*>(packet);
    if (!response) return false;
    
    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();

    player_subsystem->name_ = response->name;
    player_subsystem->body_color_ = response->body_color;
    
    player_subsystem->gm_level_ = response->gm_level;

    player_subsystem->character_id_ = response->character_id;

    player_subsystem->UpdateStat(PlayerStat::kLv, response->lv);
    player_subsystem->UpdateStat(PlayerStat::kHP, response->hp);
    player_subsystem->UpdateStat(PlayerStat::kMaxHP, response->max_hp);
    player_subsystem->UpdateStat(PlayerStat::kExp, response->exp);
    player_subsystem->UpdateStat(PlayerStat::kAtk, response->atk);
    player_subsystem->UpdateStat(PlayerStat::kDef, response->def);
    player_subsystem->UpdateStat(PlayerStat::kDig, response->dig);

    player_subsystem->map_id_ = response->map_id;
    player_subsystem->spawn_position.x = response->spawn_position.x;
    player_subsystem->spawn_position.y = response->spawn_position.y;

    player_subsystem->inventory_ = std::make_unique<Inventory>();
    Inventory* inventory = player_subsystem->inventory_.get();

    inventory->SetSlotCapacity(InventoryType::kEquip, response->equip_slot_capacity);
    inventory->SetSlotCapacity(InventoryType::kUse, response->use_slot_capacity);
    inventory->SetSlotCapacity(InventoryType::kEtc, response->etc_slot_capacity);

    for (const auto& item : response->inventory)
    {
        InventoryType type = static_cast<InventoryType>(item.inventory_type);
        inventory->AddSlot(type, item.slot_id, item.item_id, item.count);
    }
    
    inventory->SetColor(response->color);

    for (const auto& skill : response->skills)
    {
        player_subsystem->GetSkillManager()->AddOrUpdateSkill(skill.skill_id, skill.level, skill.cooldown);
    }

    SkillListUpdatedData data;
    data.skills = player_subsystem->GetSkillManager()->GetSkillList();
    PublisherSubsystem::Get()->Publish(PublisherSubsystem::EventType::kSkillsUpdated, data);

    InputActions* input_actions = InputActions::Get();
    for (const auto& key : response->key_bindings)
    {
        input_actions->Bind(key.scancode, key.type, key.action);
    }
    
    SessionSubsystem::Get()->SetState(SessionState::kInGame);
    World::Get()->OpenLevel(L"Game");
    return true;
}
