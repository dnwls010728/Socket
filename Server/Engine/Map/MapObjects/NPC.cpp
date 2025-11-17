#include "pch.h"
#include "NPC.h"

#include <CustomPacket.h>

#include "Map/PlayerCharacter.h"

NPC::NPC(uint32_t npc_id) :
    npc_id_(npc_id)
{
}

void NPC::SendSpawn(const std::shared_ptr<PlayerCharacter>& player)
{
    MapObject::SendSpawn(player);
    if (!player) return;
    
    ObjectSpawnPacket packet;
    packet.object_info.type = ObjectType::kNPC;
    packet.object_info.object_id = object_id_;
    packet.object_info.position_x = position_.x;
    packet.object_info.position_y = position_.y;
    packet.object_info.info.npc.npc_id = npc_id_;
    
    player->SendPacket(packet);
}
