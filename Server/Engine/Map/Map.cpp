#include "pch.h"
#include "Map.h"

#include <CustomPacket.h>
#include <format>
#include <ranges>

#include "Session/Player.h"
#include "tmxlite/Map.hpp"

#include "DataManager.h"
#include "MapObject.h"
#include "MapObjects/Mob/Mob.h"

Map::Map(uint32_t map_id) :
    map_id_(map_id),
    player_mutex_(),
    object_mutex_(),
    next_object_id_(1000),
    players_(),
    map_objects_(),
    pending_players_(),
    pending_remove_players_(),
    pending_objects_(),
    pending_remove_objects_(),
    footholds_(),
    mob_ids(),
    respawn_timer_(0.f),
    monitor_timer_(0.f)
{
}

void Map::AddPlayer(const std::weak_ptr<Player> &player_weak)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    pending_players_.push(player_weak);
}

void Map::RemovePlayer(uint32_t player_id)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    pending_remove_players_.push(player_id);
}

void Map::AddPlayers()
{
    std::lock_guard<std::mutex> lock(player_mutex_);

    while (!pending_players_.empty())
    {
        auto pending_player_weak = pending_players_.front();
        pending_players_.pop();
        
        auto player = pending_player_weak.lock();
        if (!player) continue;

        uint32_t object_id = player->GetCharacterID();
        players_.emplace(object_id, pending_player_weak);
    
        player->SetMap(this);
        {
            // 맵에 플레이어가 추가되면, 다른 플레이어에게 스폰하도록 패킷을 전송
            SpawnPlayerPacket spawn_player_packet;
            spawn_player_packet.character_id = player->GetCharacterID();
            spawn_player_packet.name = player->GetName();
            spawn_player_packet.position_x = player->GetPosition().x;
            spawn_player_packet.position_y = player->GetPosition().y;
            SendPacket(spawn_player_packet, player);
        }

        // 맵에 추가된 플레이어에게 다른 플레이어들을 스폰하도록 패킷을 전송
        for (const auto& player_weak : players_ | std::views::values)
        {
            auto other_player = player_weak.lock();
            if (other_player && other_player != player)
            {
                SpawnPlayerPacket spawn_player_packet;
                spawn_player_packet.character_id = other_player->GetCharacterID();
                spawn_player_packet.name = other_player->GetName();
                spawn_player_packet.position_x = other_player->GetPosition().x;
                spawn_player_packet.position_y = other_player->GetPosition().y;
                player->SendPacket(spawn_player_packet);
            }
        }

        // 맵에 추가된 플레이어에게 맵 오브젝트들을 스폰하도록 패킷을 전송
        for (const auto& map_object : map_objects_ | std::views::values)
        {
            SpawnObjectPacket spawn_object_packet;
            spawn_object_packet.object_info.type = ObjectType::kMob; // 예시로 Mob 타입으로 설정
            spawn_object_packet.object_info.object_id = map_object->GetObjectID();
            spawn_object_packet.object_info.position_x = map_object->GetPosition().x;
            spawn_object_packet.object_info.position_y = map_object->GetPosition().y;
            spawn_object_packet.object_info.info.mob = {}; // Mob 정보는 필요에 따라 설정
            player->SendPacket(spawn_object_packet);
        }
    }
}

void Map::RemovePlayers()
{
    std::lock_guard<std::mutex> lock(player_mutex_);

    while (!pending_remove_players_.empty())
    {
        uint32_t unique_key = pending_remove_players_.front();
        pending_remove_players_.pop();

        players_.erase(unique_key);
        
         // 맵에서 플레이어가 제거되면, 다른 플레이어에게 제거하도록 패킷을 전송
        DestroyPlayerPacket destroy_player_packet;
        destroy_player_packet.unique_id = unique_key;
        SendPacket(destroy_player_packet);
        
    }
}

void Map::AddObject(const std::shared_ptr<MapObject>& object)
{
    pending_objects_.push(object);
}

void Map::RemoveObject(uint32_t object_id)
{
    pending_remove_objects_.push(object_id);
}

void Map::SpawnObject(const std::shared_ptr<MapObject>& object)
{
    std::lock_guard<std::mutex> lock(object_mutex_);
    object->SetObjectID(next_object_id_++);
    object->SetMap(this);

    const auto& mob = std::dynamic_pointer_cast<Mob>(object);

    SpawnObjectPacket spawn_object_packet;
    spawn_object_packet.object_info.type = ObjectType::kMob;
    spawn_object_packet.object_info.object_id = object->GetObjectID();
    spawn_object_packet.object_info.position_x = object->GetPosition().x;
    spawn_object_packet.object_info.position_y = object->GetPosition().y;
    spawn_object_packet.object_info.info.mob.mob_id = mob->GetMobID();
    SendPacket(spawn_object_packet);

    AddObject(object);
}

void Map::DestroyObject(uint32_t object_id)
{
    std::lock_guard<std::mutex> lock(object_mutex_);
    DestroyObject_Internal(object_id);
}

void Map::SendPacket(const Net::IPacket& packet)
{
    for (auto& player_weak : std::views::values(players_))
    {
        if (auto player = player_weak.lock())
            player->SendPacket(packet);
    }
}

void Map::SendPacket(const Net::IPacket& packet, const std::weak_ptr<Player> &excluded_player_weak)
{
    auto excluded_player = excluded_player_weak.lock();
    for (const auto& player_weak : std::views::values(players_))
    {
        auto player = player_weak.lock();
        if (player && player != excluded_player)
        {
            player->SendPacket(packet);
        }
    }
}

void Map::OnAttack(uint32_t attacker_id, uint32_t defender_id)
{
    // std::lock_guard<std::mutex> lock(object_mutex_);

    {
        auto it = players_.find(attacker_id);
        if (it != players_.end())
        {
            if (auto player = it->second.lock())
            {
                player->GainExp(10000); // 예시로 10000 경험치 추가
            }
        }
    }

    auto it = map_objects_.find(defender_id);
    if (it != map_objects_.end())
    {
        Mob* mob = dynamic_cast<Mob*>(it->second.get());
        if (mob)
        {
            mob->OnHit(1000);
        }
    }
}

void Map::PhysicsTick(float delta_time)
{
    for (const auto& map_object : map_objects_ | std::views::values)
    {
        map_object->PhysicsTick(delta_time);
    }
}

void Map::Tick(float delta_time)
{
    RemovePlayers();
    AddPlayers();
    
    AddObjects();
    RemoveObjects();

    for (const auto& map_object : map_objects_ | std::views::values)
    {
        map_object->Tick(delta_time);
    }

    respawn_timer_ += delta_time;
    if (respawn_timer_ >= 10.f)
    {
        Respawn();
        respawn_timer_ -= 10.f;
    }

    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        if (players_.empty())
        {
            monitor_timer_ += delta_time;
            if (monitor_timer_ >= 5.f)
            {
                KillAllMobs();
                monitor_timer_ -= 5.f;
            }
        }
        else monitor_timer_ = 0.f;
    }

}

std::vector<std::weak_ptr<Player>> Map::GetPlayers()
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    std::vector<std::weak_ptr<Player>> players;
    for (const auto& player_weak : players_ | std::views::values)
    {
        players.push_back(player_weak);
    }
    return players;
}

bool Map::LoadMapData()
{
    std::string path = std::format(".\\Content\\Tilemaps\\{:06}.tmx", map_id_);
        
    tmx::Map map_data;
    if (!map_data.load(path)) return false;

    const auto& properties = map_data.getProperties();
    if (properties.empty()) return false;

    float ppu = properties[1].getFloatValue();

    const auto& layers = map_data.getLayers();
    for (const auto& layer : layers)
    {
        if (layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& object_group = layer->getLayerAs<tmx::ObjectGroup>();

            if (layer->getName() == "Foothold")
            {
                const auto& objects = object_group.getObjects();
                for (const auto& object : objects)
                {
                    if (object.getShape() == tmx::Object::Shape::Polyline)
                    {
                        const auto& points = object.getPoints();
                        for (size_t i = 0; i < points.size() - 1; ++i)
                        {
                            Math::Vector2 point1 = {
                                points[i].x / ppu + object.getPosition().x / ppu - map_data.getTileCount().x / 2.f,
                                -1 * points[i].y / ppu - object.getPosition().y / ppu + map_data.getTileCount().y / 2.f
                            };
                            
                            Math::Vector2 point2 = {
                                points[i + 1].x / ppu + object.getPosition().x / ppu - map_data.getTileCount().x / 2.f,
                                -1 * points[i + 1].y / ppu - object.getPosition().y / ppu + map_data.getTileCount().y / 2.f
                            };

                            footholds_.emplace_back(std::make_unique<Foothold>(point1, point2));
                        }
                    }
                }
            }
            else if (layer->getName() == "SpawnPoint")
            {
                const auto& objects = object_group.getObjects();
                for (const auto& object : objects)
                {
                    if (object.getShape() != tmx::Object::Shape::Point) continue;
                    
                    Math::Vector2 position = {
                        object.getPosition().x / ppu - map_data.getTileCount().x / 2.f,
                        -1 * object.getPosition().y / ppu + map_data.getTileCount().y / 2.f
                    };

                    const auto& properties = object.getProperties();
                    if (properties.empty()) continue;
                    
                    spawn_points_.emplace_back(position, properties[0].getIntValue());
                }
            }
        }
    }

    return true;
}

Foothold* Map::FindFoothold(const Math::Vector2& position)
{
    Foothold* best = nullptr;
    float best_y = -std::numeric_limits<float>::max();
    
    for (const auto& foothold : footholds_)
    {
        if (position.x < foothold->GetX1() || position.x > foothold->GetX2()) continue;
        
        float y = foothold->GetYAt(position.x);
        if (best_y <= y && position.y >= y)
        {
            best_y = y;
            best = foothold.get();
        }
    }

    return best;
}

void Map::AddObjects()
{
    std::lock_guard<std::mutex> lock(object_mutex_);
    while (!pending_objects_.empty())
    {
        std::shared_ptr<MapObject> object = pending_objects_.front();
        pending_objects_.pop();

        object->BeginPlay();
        map_objects_.emplace(object->GetObjectID(), object);
    }
}

void Map::RemoveObjects()
{
    std::lock_guard<std::mutex> lock(object_mutex_);
    while (!pending_remove_objects_.empty())
    {
        uint32_t object_id = pending_remove_objects_.front();
        pending_remove_objects_.pop();

        map_objects_.erase(object_id);
    }
}

void Map::Respawn()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        if (players_.empty()) return;
    }

    for (const auto& spawn_point : spawn_points_)
    {
        if (const MobData* mob_data = DataManager::Get()->GetMobData(spawn_point.mob_id))
        {
            std::shared_ptr<Mob> mob = std::make_shared<Mob>(*mob_data);
            mob->SetPosition(spawn_point.position);
            mob->SetLastPosition(spawn_point.position);
            SpawnObject(mob);
        }
    }
}

void Map::KillAllMobs()
{
    std::lock_guard<std::mutex> lock(object_mutex_);
    for (const auto& map_object : map_objects_ | std::views::values)
    {
        if (const auto& mob = std::dynamic_pointer_cast<Mob>(map_object))
        {
            DestroyObject_Internal(mob->GetObjectID());
        }
    }
}

void Map::DestroyObject_Internal(uint32_t object_id)
{
    auto it = map_objects_.find(object_id);
    if (it == map_objects_.end()) return;

    DestroyObjectPacket destroy_object_packet;
    destroy_object_packet.object_id = object_id;
    SendPacket(destroy_object_packet);

    RemoveObject(object_id);
}
