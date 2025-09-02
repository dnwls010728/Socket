#include "pch.h"
#include "SelectCharacterHandler.h"

#include <CustomPacket.h>

#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"

bool SelectCharacterHandler::Handle(Net::IPacket* packet)
{
    SelectCharacterResponse* response = dynamic_cast<SelectCharacterResponse*>(packet);
    if (!response) return false;
    
    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();

    player_subsystem->name_ = response->name;
    player_subsystem->body_color_ = response->body_color;

    player_subsystem->character_id_ = response->character_id;

    player_subsystem->UpdateStat(PlayerStat::kLv, response->lv);
    player_subsystem->UpdateStat(PlayerStat::kHP, response->hp);
    player_subsystem->UpdateStat(PlayerStat::kMaxHP, response->max_hp);
    player_subsystem->UpdateStat(PlayerStat::kExp, response->exp);

    player_subsystem->map_id_ = response->map_id;
    player_subsystem->spawn_position.x = response->spawn_position.x;
    player_subsystem->spawn_position.y = response->spawn_position.y;

    player_subsystem->inventory_ = std::make_unique<OLD_Inventory>();
    OLD_Inventory* inventory = player_subsystem->inventory_.get();

    inventory->SetSlotCapacity(OLD_Inventory::Type::kEquip, response->equip_slot_capacity);
    inventory->SetSlotCapacity(OLD_Inventory::Type::kUse, response->use_slot_capacity);
    inventory->SetSlotCapacity(OLD_Inventory::Type::kEtc, response->etc_slot_capacity);

    for (const auto& item : response->inventory)
    {
        OLD_Inventory::Type type = static_cast<OLD_Inventory::Type>(item.inventory_type);
        inventory->AddSlot(type, item.slot_id, item.item_id, item.count);
    }
    
    inventory->SetColor(response->color);
    
    SessionSubsystem::Get()->SetState(SessionState::kInGame);
    World::Get()->OpenLevel(L"Game");
    return true;
}
