#include "pch.h"
#include "DroppedItem.h"

#include <CustomPacket.h>

#include "Map/PlayerCharacter.h"

DroppedItem::DroppedItem() :
    dropper_(),
    item_id_(0),
    count_(0)
{
}

void DroppedItem::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);
    if (!player) return;

    SpawnObjectPacket packet;
    packet.object_info.type = ObjectType::kDroppedItem;
    packet.object_info.object_id = GetObjectID();
    packet.object_info.position_x = GetPosition().x;
    packet.object_info.position_y = GetPosition().y;

    DroppedItemInfo& info = packet.object_info.info.dropped_item;
    info.item_id = item_id_;
    info.dropper_position_x = 0.f;
    info.dropper_position_y = 0.f;
    
    player->SendPacket(packet);
}
