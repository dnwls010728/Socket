#include "pch.h"
#include "Block.h"

#include <CustomPacket.h>

#include "Map/PlayerCharacter.h"

Block::Block(const std::wstring& color, int32_t hp) :
    color_(color),
    hp_(hp)
{
}

void Block::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);
    if (!player) return;

    ObjectSpawnPacket packet;
    packet.object_info.type = ObjectType::kBlock;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;

    BlockInfo& info = packet.object_info.info.block;
    wcscpy_s(info.color, color_.c_str());
    
    player->SendPacket(packet);
}
