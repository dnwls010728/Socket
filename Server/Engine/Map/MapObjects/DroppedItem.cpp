#include "pch.h"
#include "DroppedItem.h"

#include <CustomPacket.h>
#include <iostream>

#include "NetDef.h"
#include "Map/Map.h"
#include "Map/PlayerCharacter.h"
#include "Session/Player/Inventory/Item.h"

DroppedItem::DroppedItem() :
    dropper_(),
    item_(nullptr),
    color_(0),
    dropped_time_(0.)
{
}

void DroppedItem::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);
    if (!player) return;

    ObjectSpawnPacket packet;
    packet.object_info.type = ObjectType::kDroppedItem;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    DroppedItemInfo& info = packet.object_info.info.dropped_item;
    info.item_id = item_ ? item_->GetID() : 0;
    info.dropper_position_x = position_.x;
    info.dropper_position_y = position_.y;
    info.color = color_;
    
    player->SendPacket(packet);
}

void DroppedItem::Tick(float delta_time)
{
    MapObject::Tick(delta_time);
    
    if (!map_) return;
    if (dropped_time_ < Net::GetClientTime())
    {
        map_->DestroyDroppedItem(object_id_, 0);
        return;
    }
}
