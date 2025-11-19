#include "pch.h"
#include "ObjectSpawnHandler.h"

#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actors/DroppedItem.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Mobs/MobBase.h"
#include "Actors/Projectile.h"
#include "Actors/Characters/NPC.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/ObjectPool/ObjectPoolSubsystem.h"

bool ObjectSpawnHandler::Handle(Net::IPacket* packet)
{
    ObjectSpawnPacket* received_packet = dynamic_cast<ObjectSpawnPacket*>(packet);
    if (!received_packet) return false;

    const ObjectInfo& object_info = received_packet->object_info;

    switch (object_info.type)
    {
    case ObjectType::kPlayer:
        {
            NetworkSubsystem* network_subsystem = NetworkSubsystem::Get();
            std::shared_ptr<PlayerCharacter> player_character = network_subsystem->SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), object_info.object_id);
            if (IsValid(player_character))
            {
                player_character->Init(object_info.info.player.name, object_info.info.player.body_color, {object_info.position_x, object_info.position_y}, object_info.info.player.gm_level);
                network_subsystem->other_players_.emplace_back(player_character);
            }
        }
        break;
    case ObjectType::kMob:
        {
            std::shared_ptr<Actor> out_actor = nullptr;
            if (!ObjectPoolSubsystem::Get()->GetFromPool(MobBase::StaticClass(), out_actor)) return false;
            if (auto mob = std::dynamic_pointer_cast<MobBase>(out_actor))
            {
                mob->SetObjectID(object_info.object_id);
                mob->GetTransform()->SetPosition({ object_info.position_x, object_info.position_y });
                
                mob->Init(object_info.info.mob.mob_id);
                mob->SetFlip((object_info.info.mob.is_fliped));
                mob->PlayAnimation(object_info.info.mob.animation_name);
            }
        }
        break;

    case ObjectType::kDroppedItem:
        {
            std::shared_ptr<Actor> out_actor = nullptr;
            if (!ObjectPoolSubsystem::Get()->GetFromPool(DroppedItem::StaticClass(), out_actor)) return false;
            if (auto dropped_item = std::dynamic_pointer_cast<DroppedItem>(out_actor))
            {
                if (!object_info.info.dropped_item.color)
                {
                    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\itemdrop.mp3");
                    AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
                }
                else
                {
                    Audio* audio = AssetManager::Get()->Load<Audio>(L"Audio\\SE\\moneydrop.mp3");
                    AudioManager::Get()->PlaySound2D(audio, ChannelGroup::kSE);
                }

                DroppedItemInfo info = object_info.info.dropped_item;

                Math::Vector2 offset = Math::Vector2::Up() * .25f;

                dropped_item->SetObjectID(object_info.object_id);
                dropped_item->GetTransform()->SetPosition({ object_info.position_x, object_info.position_y + offset.y });

                dropped_item->Init(info.item_id, info.color, { object_info.position_x, object_info.position_y + offset.y });
            }
        }
        break;
    case ObjectType::kProjectile:
        {
            std::shared_ptr<Actor> out_actor = nullptr;
            if (!ObjectPoolSubsystem::Get()->GetFromPool(Projectile::StaticClass(), out_actor)) return false;
            if (auto projectile = std::dynamic_pointer_cast<Projectile>(out_actor))
            {
                projectile->SetObjectID(object_info.object_id);
                projectile->GetTransform()->SetPosition({ object_info.position_x, object_info.position_y });

                projectile->Init(object_info.info.projectile.projectile_id);
                projectile->SetFlip(object_info.info.projectile.is_flipped);
                projectile->PlayAnimation(object_info.info.projectile.animation_name);
            }
        }
        break;
    case ObjectType::kNPC:
        {
            NPCInfo npc_info = object_info.info.npc;
            
            NetworkSubsystem* network_subsystem = NetworkSubsystem::Get();
            std::shared_ptr<NPC> npc = network_subsystem->SpawnNetworkActor<NPC>(NPC::StaticClass(), object_info.object_id);
            if (IsValid(npc)) npc->Init(npc_info.npc_id, {object_info.position_x, object_info.position_y});
        }
        break;
    }

    return true;
}
