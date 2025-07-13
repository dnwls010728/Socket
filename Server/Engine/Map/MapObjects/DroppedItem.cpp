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
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    DroppedItemInfo& info = packet.object_info.info.dropped_item;
    info.item_id = item_id_;
    info.dropper_position_x = position_.x;
    info.dropper_position_y = position_.y;
    
    player->SendPacket(packet);
}
