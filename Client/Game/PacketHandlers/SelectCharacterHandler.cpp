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
    player_subsystem->lv_ = response->lv;
    
    player_subsystem->initial_position_x_ = response->position_x;
    player_subsystem->initial_position_y_ = response->position_y;

    player_subsystem->inventory_ = std::make_unique<Inventory>();
    Inventory* inventory = player_subsystem->inventory_.get();

    for (const auto& item : response->inventory)
    {
        inventory->AddSlot(item.slot_index, item.item_id, item.count);
    }
    
    player_subsystem->inventory_->SetColor(response->color);
    
    SessionSubsystem::Get()->SetState(SessionState::kInGame);
    World::Get()->OpenLevel(L"InGame");
    return true;
}
