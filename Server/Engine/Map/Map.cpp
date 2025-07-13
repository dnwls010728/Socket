#include "pch.h"
#include "Map.h"

#include <CustomPacket.h>
#include <format>
#include <ranges>

#include "tmxlite/Map.hpp"

#include "DataManager.h"
#include "MapObject.h"
#include "PlayerCharacter.h"
#include "MapObjects/DroppedItem.h"
#include "MapObjects/Mob/Mob.h"
#include "Math/Math.h"

Map::Map(uint32_t map_id) :
    map_id_(map_id),
    map_bounds_(),
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

void Map::AddPlayer(const std::weak_ptr<PlayerCharacter> &player_weak)
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

        uint32_t object_id = player->GetObjectID();
        players_.emplace(object_id, pending_player_weak);
    
        player->SetMap(this);
        {
            for (const auto& player_weak : players_ | std::views::values)
            {
                auto other_player = player_weak.lock();
                if (other_player && other_player != player)
                {
                    player->SendSpawn(other_player);
                }
            }
        }

        // 맵에 추가된 플레이어에게 다른 플레이어들을 스폰하도록 패킷을 전송
        for (const auto& player_weak : players_ | std::views::values)
        {
            auto other_player = player_weak.lock();
            if (other_player && other_player != player)
            {
                other_player->SendSpawn(player);
            }
        }

        // 맵에 추가된 플레이어에게 맵 오브젝트들을 스폰하도록 패킷을 전송
        for (const auto& map_object : map_objects_ | std::views::values)
        {
            if (map_object) map_object->SendSpawn(player);
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
    object->SetObjectID(next_object_id_.fetch_add(1));
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
    for (auto& player_weak : players_ | std::views::values)
    {
        if (auto player = player_weak.lock())
            player->SendPacket(packet);
    }
}

void Map::SendPacket(const Net::IPacket& packet, const std::weak_ptr<PlayerCharacter> &excluded_player_weak)
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

void Map::OnAttack(uint32_t attacker, uint32_t defender)
{
    // std::lock_guard<std::mutex> lock(object_mutex_);

    auto it = map_objects_.find(defender);
    if (it != map_objects_.end())
    {
        Mob* mob = dynamic_cast<Mob*>(it->second.get());
        if (mob)
        {
            mob->OnHit(attacker, 1000);
        }
    }
}

void Map::PhysicsTick(float delta_time)
{
    for (const auto& player_weak : std::views::values(players_))
    {
        auto player_shared = player_weak.lock();
        if (!player_shared) continue;
        
        player_shared->PhysicsTick(delta_time);
    }
    
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

    for (const auto& player_weak : std::views::values(players_))
    {
        auto player_shared = player_weak.lock();
        if (!player_shared) continue;
        
        player_shared->Tick(delta_time);
    }

    for (const auto& map_object : map_objects_ | std::views::values)
    {
        map_object->Tick(delta_time);
    }

    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        for (const auto& player_weak : players_ | std::views::values)
        {
            auto player = player_weak.lock();
            if (!player) continue;

            for (const auto& map_object : map_objects_ | std::views::values)
            {
                const auto& mob = std::dynamic_pointer_cast<Mob>(map_object);
                if (!mob) continue;

                float distance = Math::Vector2::Distance(player->GetPosition(), mob->GetPosition());
                if (distance < 1.f && !player->IsInvincible())
                {
                    player->TakeDamage(mob->damage_);
                    break;
                }
            }
        }
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

void Map::SpawnDropItem(uint32_t item_id, uint32_t count, const std::shared_ptr<MapObject>& dropper, const Math::Vector2& drop_position)
{
    std::lock_guard<std::mutex> lock(object_mutex_);
    
    std::shared_ptr<DroppedItem> dropped_item = std::make_shared<DroppedItem>();
    dropped_item->SetDropper(dropper);
    dropped_item->SetItemID(item_id);
    dropped_item->SetCount(count);
    dropped_item->SetPosition(drop_position);

    // TODO: 구조 개선 필요
    dropped_item->SetObjectID(next_object_id_.fetch_add(1));
    dropped_item->SetMap(this);

    SpawnObjectPacket packet;
    packet.object_info.type = ObjectType::kDroppedItem;
    packet.object_info.object_id = dropped_item->GetObjectID();
    packet.object_info.position_x = drop_position.x;
    packet.object_info.position_y = drop_position.y;

    DroppedItemInfo& info = packet.object_info.info.dropped_item;
    info.item_id = item_id;
    info.dropper_position_x = dropper->GetPosition().x;
    info.dropper_position_y = dropper->GetPosition().y;
    SendPacket(packet);
    
    AddObject(dropped_item);
}

std::vector<std::weak_ptr<PlayerCharacter>> Map::GetPlayers()
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    std::vector<std::weak_ptr<PlayerCharacter>> players;
    for (const auto& player_weak : players_ | std::views::values)
    {
        players.push_back(player_weak);
    }
    return players;
}

Math::Vector2 Map::GetDropPosition(const Math::Vector2& position)
{
    Math::Vector2 drop_position = { position.x, position.y + 2.f };
    drop_position.x = Math::Clamp(drop_position.x, map_bounds_.min.x, map_bounds_.max.x);

    Foothold* foothold = FindFoothold(drop_position);
    if (foothold) drop_position.y = foothold->GetYAt(drop_position.x);
    
    return drop_position;
}

bool Map::LoadMapData()
{
    std::string path = std::format(".\\Content\\Tilemaps\\{:06}.tmx", map_id_);
        
    tmx::Map map_data;
    if (!map_data.load(path)) return false;

    const auto& properties = map_data.getProperties();
    if (properties.empty()) return false;

    float ppu = properties[1].getFloatValue();

    tmx::FloatRect local_bounds = map_data.getBounds();
    float world_width = local_bounds.width / ppu;
    float world_height = local_bounds.height / ppu;

    map_bounds_ = { Math::Vector2::Zero(), { world_width, world_height } };

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

const std::shared_ptr<PlayerCharacter>& Map::FindPlayer(uint32_t player_id)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    
    auto it = players_.find(player_id);
    if (it == players_.end()) return nullptr;
    return it->second.lock();
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
