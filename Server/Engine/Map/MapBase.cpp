#include "MapBase.h"

#include <CustomPacket.h>
#include <ranges>

#include "Actor/Actor.h"
#include "Session/Player.h"
#include "Engine/Misc/Type.h"
#include "Engine/Enums.h"

MapBase::MapBase(uint32_t MapBase_id) :
    map_unique_id_(MapBase_id)
{
}

void MapBase::AddPlayer(Player* player)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    players_.push_back(player);

    {
        // 맵에 플레이어가 추가되면, 다른 플레이어에게 스폰하도록 패킷을 전송
        SpawnPlayerPacket spawn_player_packet;
        spawn_player_packet.character_info = player->GetCharacterInfo();
        spawn_player_packet.position_x = player->GetPositionX();
        spawn_player_packet.position_y = player->GetPositionY();
        SendPacket(spawn_player_packet, player);
    }

    // 맵에 추가된 플레이어에게 다른 플레이어들을 스폰하도록 패킷을 전송
    for (auto& other_player : players_)
    {
        if (other_player && other_player != player)
        {
            SpawnPlayerPacket spawn_player_packet;
            spawn_player_packet.character_info = other_player->GetCharacterInfo();
            spawn_player_packet.position_x = other_player->GetPositionX();
            spawn_player_packet.position_y = other_player->GetPositionY();
            player->SendPacket(spawn_player_packet);
        }
    }

    for (auto& map_object :  std::views::values(map_objects_))
    {
        SpawnObjectPacket spawn_object_packet;
        spawn_object_packet.object.unique_id = map_object->GetUniqueID();
        spawn_object_packet.object.last_position_x = map_object->GetUniqueID();
        
    }
}

void MapBase::RemovePlayer(Player* player)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    std::erase(players_, player);

    {
        // 맵에서 플레이어가 제거되면, 다른 플레이어에게 제거하도록 패킷을 전송
        DestroyPlayerPacket destroy_player_packet;
        destroy_player_packet.unique_id = player->GetCharacterInfo().unique_id;
        SendPacket(destroy_player_packet, player);
    }
}

void MapBase::SendPacket(const Net::IPacket& packet)
{
    for (auto& player : players_)
    {
        if (player) player->SendPacket(packet);
    }
}

void MapBase::SendPacket(const Net::IPacket& packet, const Player* excluded_player)
{
    for (const auto& player : players_)
    {
        if (player && player != excluded_player)
        {
            player->SendPacket(packet);
        }
    }
}

void MapBase::Tick(float delta_time)
{
    SpawnActors();
    ProcessActorActivation();
    DestroyActors();
}

void MapBase::PhysicsTick(float delta_time)
{
}

void MapBase::PostTick(float delta_time)
{
}

void MapBase::TransitionLevel()
{
}

void MapBase::ProcessActorActivation()
{
    while (!pending_actor_activation_.empty())
    {
        const ActorActivation& activation = pending_actor_activation_.front();
        pending_actor_activation_.pop();
        
        Actor* actor = activation.actor;
        
        if (activation.is_active) actor->OnEnable();
        else actor->OnDisable();
    }
}

void MapBase::SpawnActors()
{
    while (!pending_actors_.empty())
    {
        std::shared_ptr<Actor> actor = pending_actors_.front();
        map_objects_.insert({actor->GetUniqueID(),actor});
        actor->BeginPlay();
        
        pending_actors_.pop();
    }
}

void MapBase::DestroyActor(Actor* actor)
{
    //actor->is_pending_destroy_ = true;
    
    std::shared_ptr<Actor> shared_actor = actor->GetSharedThis();
    pending_destroy_actors_.push(shared_actor);
}

void MapBase::DestroyActors()
{
    while (!pending_destroy_actors_.empty())
    {
        std::shared_ptr<Actor> actor = pending_destroy_actors_.front();
        actor->EndPlay(EndPlayReason::kDestroyed);
        
        map_objects_.erase(actor->GetUniqueID());
        pending_destroy_actors_.pop();
    }
}

void MapBase::ActivateActor(Actor* actor, bool is_active)
{
    pending_actor_activation_.push({actor, is_active});
}
