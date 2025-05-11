#include "pch.h"
#include "Map.h"

#include <CustomPacket.h>
#include <ranges>

#include "Actor/Actor.h"
#include "Session/Player.h"
#include "Engine/Misc/Type.h"
#include "Engine/Enums.h"
#include "box2d/box2d.h"

Map::Map(uint32_t MapBase_id) :
    map_unique_id_(MapBase_id),
    world_id_(b2_nullWorldId)
{
}

Map::~Map()
{
    b2DestroyWorld(world_id_);
    world_id_ = b2_nullWorldId;
}

void Map::Init()
{
    InitPhysicsWorld();
}

void Map::AddPlayer(Player* player)
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

void Map::RemovePlayer(Player* player)
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

void Map::SendPacket(const Net::IPacket& packet)
{
    for (auto& player : players_)
    {
        if (player) player->SendPacket(packet);
    }
}

void Map::SendPacket(const Net::IPacket& packet, const Player* excluded_player)
{
    for (const auto& player : players_)
    {
        if (player && player != excluded_player)
        {
            player->SendPacket(packet);
        }
    }
}

void Map::Tick(float delta_time)
{
    SpawnActors();
    ProcessActorActivation();
    DestroyActors();

    for (const auto& actor : map_objects_)
    {
        actor.second->Tick(delta_time);
    }
}

void Map::PhysicsTick(float delta_time)
{
    b2World_Step(world_id_, delta_time, 4);
    for (const auto& actor : map_objects_)
    {
        actor.second->PhysicsTick(delta_time);
    }
    ProcessCollisionEvents();
    ProcessTriggerEvents();
    
    ProcessActorActivation();
    DestroyActors();
}

void Map::PostTick(float delta_time)
{
    for (const auto& actor : map_objects_)
    {
        actor.second->PostTick(delta_time);
    }

    DestroyActors();
    ProcessActorActivation();
}

void Map::TransitionLevel()
{
}

void Map::ProcessActorActivation()
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

void Map::SpawnActors()
{
    while (!pending_actors_.empty())
    {
        std::shared_ptr<Actor> actor = pending_actors_.front();
        map_objects_.insert({actor->GetUniqueID(),actor});
        actor->BeginPlay();
        
        pending_actors_.pop();
    }
}

void Map::DestroyActor(Actor* actor)
{
    actor->is_pending_destroy_ = true;
    
    std::shared_ptr<Actor> shared_actor = actor->GetSharedThis();
    pending_destroy_actors_.push(shared_actor);
}

void Map::InitPhysicsWorld()
{
    b2Vec2 gravity(0.f, -20.f);
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = gravity;

    world_id_ = b2CreateWorld(&world_def);
}

void Map::DestroyActors()
{
    while (!pending_destroy_actors_.empty())
    {
        std::shared_ptr<Actor> actor = pending_destroy_actors_.front();
        actor->EndPlay(EndPlayReason::kDestroyed);
        
        map_objects_.erase(actor->GetUniqueID());
        pending_destroy_actors_.pop();
    }
}

void Map::ActivateActor(Actor* actor, bool is_active)
{
    pending_actor_activation_.push({actor, is_active});
}

void Map::ProcessTriggerEvents()
{
    b2SensorEvents events = b2World_GetSensorEvents(world_id_);
    for (uint32_t i = 0; i < events.beginCount; ++i)
    {
        b2SensorBeginTouchEvent event = events.beginEvents[i];
        if (!b2Shape_IsValid(event.sensorShapeId) || !b2Shape_IsValid(event.visitorShapeId)) continue;
        
        b2BodyId body_id_a = b2Shape_GetBody(event.sensorShapeId);
        b2BodyId body_id_b = b2Shape_GetBody(event.visitorShapeId);
        
        if (!b2Body_IsValid(body_id_a) || !b2Body_IsValid(body_id_b)) continue;
        
        Actor* actor_a = static_cast<Actor*>(b2Body_GetUserData(body_id_a));
        Actor* actor_b = static_cast<Actor*>(b2Body_GetUserData(body_id_b));
        
        if (!IsValid(actor_a) || !IsValid(actor_b)) continue;
        actor_a->OnTriggerEnter(actor_b);
        actor_b->OnTriggerEnter(actor_a);
    }
    
    for (uint32_t i = 0; i < events.endCount; ++i)
    {
        b2SensorEndTouchEvent event = events.endEvents[i];
        if (!b2Shape_IsValid(event.sensorShapeId) || !b2Shape_IsValid(event.visitorShapeId)) continue;
        
        b2BodyId body_id_a = b2Shape_GetBody(event.sensorShapeId);
        b2BodyId body_id_b = b2Shape_GetBody(event.visitorShapeId);
        
        if (!b2Body_IsValid(body_id_a) || !b2Body_IsValid(body_id_b)) continue;
        
        Actor* actor_a = static_cast<Actor*>(b2Body_GetUserData(body_id_a));
        Actor* actor_b = static_cast<Actor*>(b2Body_GetUserData(body_id_b));
        
        if (!IsValid(actor_a) || !IsValid(actor_b)) continue;
        actor_a->OnTriggerExit(actor_b);
        actor_b->OnTriggerExit(actor_a);
    }
}

void Map::ProcessCollisionEvents()
{
    b2ContactEvents events = b2World_GetContactEvents(world_id_);
    for (uint32_t i = 0; i < events.beginCount; ++i)
    {
        b2ContactBeginTouchEvent event = events.beginEvents[i];
        if (!b2Shape_IsValid(event.shapeIdA) || !b2Shape_IsValid(event.shapeIdB)) continue;
        
        b2BodyId body_id_a = b2Shape_GetBody(event.shapeIdA);
        b2BodyId body_id_b = b2Shape_GetBody(event.shapeIdB);

        if (!b2Body_IsValid(body_id_a) || !b2Body_IsValid(body_id_b)) continue;

        Actor* actor_a = static_cast<Actor*>(b2Body_GetUserData(body_id_a));
        Actor* actor_b = static_cast<Actor*>(b2Body_GetUserData(body_id_b));

        if (!IsValid(actor_a) || !IsValid(actor_b)) continue;
        
        actor_a->OnCollisionEnter(actor_b);
        actor_b->OnCollisionEnter(actor_a);
    }

    for (uint32_t i = 0; i < events.endCount; ++i)
    {
        b2ContactEndTouchEvent event = events.endEvents[i];
        if (!b2Shape_IsValid(event.shapeIdA) || !b2Shape_IsValid(event.shapeIdB)) continue;
        
        b2BodyId body_id_a = b2Shape_GetBody(event.shapeIdA);
        b2BodyId body_id_b = b2Shape_GetBody(event.shapeIdB);
        
        if (!b2Body_IsValid(body_id_a) || !b2Body_IsValid(body_id_b)) continue;

        Actor* actor_a = static_cast<Actor*>(b2Body_GetUserData(body_id_a));
        Actor* actor_b = static_cast<Actor*>(b2Body_GetUserData(body_id_b));

        if (!IsValid(actor_a) || !IsValid(actor_b)) continue;
        
        actor_a->OnCollisionExit(actor_b);
        actor_b->OnCollisionExit(actor_a);
    }
}
