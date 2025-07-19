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

    player_subsystem->character_id_ = response->character_id;

    player_subsystem->UpdateStat(PlayerStat::kLv, response->lv);
    player_subsystem->UpdateStat(PlayerStat::kHP, response->hp);
    player_subsystem->UpdateStat(PlayerStat::kMaxHP, response->max_hp);
    player_subsystem->UpdateStat(PlayerStat::kExp, response->exp);

    player_subsystem->map_id_ = response->map_id;
    player_subsystem->spawn_position.x = response->spawn_position.x;
    player_subsystem->spawn_position.y = response->spawn_position.y;

    player_subsystem->inventory_ = std::make_unique<Inventory>();
    Inventory* inventory = player_subsystem->inventory_.get();

    for (const auto& item : response->inventory)
    {
        Inventory::Type type = static_cast<Inventory::Type>(item.inventory_type);
        inventory->AddSlot(type, item.slot_index, item.item_id, item.count);
    }
    
    player_subsystem->inventory_->SetColor(response->color);
    
    SessionSubsystem::Get()->SetState(SessionState::kInGame);
    World::Get()->OpenLevel(L"Game");
    return true;
}
