#include "pch.h"
#include "NPC.h"

#include <CustomPacket.h>

#include "Map/PlayerCharacter.h"
#include "Shop/ShopManager.h"

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

void NPC::SendShop(const std::shared_ptr<PlayerCharacter>& player) const
{
    std::shared_ptr<Shop> shop = ShopManager::Get()->GetShopByNPC(npc_id_);
    if (!shop) return;

    shop->SendShop(player);
}

bool NPC::HasShop() const
{
    return ShopManager::Get()->GetShopByNPC(npc_id_) != nullptr;
}
