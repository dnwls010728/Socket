#include "pch.h"
#include "Map.h"

#include <CustomPacket.h>
#include <format>
#include <ranges>

#include "tmxlite/Map.hpp"

#include <random>

#include "DataManager.h"
#include "MapObject.h"
#include "NetDef.h"
#include "PlayerCharacter.h"
#include "SpawnPoint.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "MapObjects/Block.h"
#include "MapObjects/DroppedItem.h"
#include "MapObjects/Mob/Mob.h"
#include "Math/Math.h"
#include "MySQL/MySQLManager.h"
#include "Session/Player/Inventory/EquipItem.h"
#include "Session/Player/Inventory/Item.h"


// TODO: 오브젝트도 플레이어와 동일하게 대기 후 Tick에서 패킷을 전송하도록 변경 필요
Map::Map(uint32_t map_id) :
    map_id_(map_id),
    return_map_id_(0),
    map_bounds_(),
    player_mutex_(),
    object_mutex_(),
    next_object_id_(1000),
    number_spawned_mobs_(0),
    players_(),
    footholds_(),
    portals_(),
    map_objects_(),
    pending_players_(),
    pending_remove_players_(),
    pending_objects_(),
    pending_remove_objects_(),
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
    
        player->SetMapID(map_id_);
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
    std::vector<uint32_t> removed_players;

    {
        std::lock_guard<std::mutex> lock(player_mutex_);

        while (!pending_remove_players_.empty())
        {
            uint32_t unique_key = pending_remove_players_.front();
            pending_remove_players_.pop();

            players_.erase(unique_key);
            removed_players.push_back(unique_key);
        }
    }

    for (uint32_t unique_key : removed_players)
    {
        ObjectDestroyInfo info;
        info.type = ObjectType::kPlayer;
        info.object_id = unique_key;

        ObjectDestroyPacket destroy_player_packet;
        destroy_player_packet.object_info = info;

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

void Map::SpawnMob(const std::shared_ptr<MapObject>& object)
{
    const auto& mob = std::dynamic_pointer_cast<Mob>(object);
    if (!mob) return;
    
    mob->SetObjectID(next_object_id_.fetch_add(1));
    mob->SetMap(this);
    mob->OnDeath(this, &Map::OnMobDeath);
    
    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        AddObject(mob);
    }
    
    ObjectInfo info;
    info.type = ObjectType::kMob;
    info.object_id = mob->GetObjectID();
    info.position_x = mob->GetPosition().x;
    info.position_y = mob->GetPosition().y;
    info.info.mob.mob_id = mob->GetMobID();
    info.info.mob.is_fliped = mob->IsFlipped();
    wcscpy_s(info.info.mob.animation_name, mob->GetAnimation().c_str());

    ObjectSpawnPacket packet;
    packet.object_info = info;
    
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        SendPacket(packet);
    }

    number_spawned_mobs_.fetch_add(1);
}

void Map::SpawnColorDrop(int32_t color, const std::shared_ptr<MapObject>& dropper, const Math::Vector2& drop_position)
{
    std::shared_ptr<DroppedItem> dropped_item = std::make_shared<DroppedItem>();
    dropped_item->SetDropper(dropper);
    dropped_item->SetColor(color);

    dropped_item->SetObjectID(next_object_id_.fetch_add(1));
    dropped_item->SetMap(this);
    dropped_item->SetPosition(drop_position);

    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        AddObject(dropped_item);
    }

    DroppedItemInfo item_info;
    item_info.item_id = 0;
    item_info.dropper_position_x = dropper->GetPosition().x;
    item_info.dropper_position_y = dropper->GetPosition().y;
    item_info.color = color;

    ObjectInfo info;
    info.type = ObjectType::kDroppedItem;
    info.object_id = dropped_item->GetObjectID();
    info.position_x = drop_position.x;
    info.position_y = drop_position.y;
    info.info.dropped_item = item_info;

    ObjectSpawnPacket packet;
    packet.object_info = info;
    
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        SendPacket(packet);
    }
}

void Map::SpawnItemDrop(const std::shared_ptr<Item>& item, const std::shared_ptr<MapObject>& dropper, const Math::Vector2& drop_position)
{
    std::shared_ptr<DroppedItem> dropped_item = std::make_shared<DroppedItem>();
    dropped_item->SetDropper(dropper);
    dropped_item->SetItem(item);

    dropped_item->SetObjectID(next_object_id_.fetch_add(1));
    dropped_item->SetMap(this);
    dropped_item->SetPosition(drop_position);

    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        AddObject(dropped_item);
    }

    DroppedItemInfo item_info;
    item_info.item_id = item->GetID();
    item_info.dropper_position_x = dropper->GetPosition().x;
    item_info.dropper_position_y = dropper->GetPosition().y;
    item_info.color = 0;

    ObjectInfo info;
    info.type = ObjectType::kDroppedItem;
    info.object_id = dropped_item->GetObjectID();
    info.position_x = drop_position.x;
    info.position_y = drop_position.y;
    info.info.dropped_item = item_info;

    ObjectSpawnPacket packet;
    packet.object_info = info;
    
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        SendPacket(packet);
    }
}

void Map::SpawnBlock(const std::wstring& color, int32_t hp, const Math::Vector2& position)
{
    std::shared_ptr<Block> block = std::make_shared<Block>(color, hp);
    block->SetObjectID(next_object_id_.fetch_add(1));
    block->SetMap(this);
    block->SetPosition(position);
    
    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        AddObject(block);
    }

    BlockInfo block_info;
    wcscpy_s(block_info.color, color.c_str());
    
    ObjectInfo info;
    info.type = ObjectType::kBlock;
    info.object_id = block->GetObjectID();
    info.position_x = position.x;
    info.position_y = position.y;
    info.info.block = block_info;
    
    ObjectSpawnPacket packet;
    packet.object_info = info;
    
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        SendPacket(packet);
    }
}

void Map::DestroyMob(uint32_t object_id)
{
    ObjectDestroyInfo info;
    info.type = ObjectType::kMob;
    info.object_id = object_id;
    
    ObjectDestroyPacket object_destroy_packet;
    object_destroy_packet.object_info = info;
    
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        SendPacket(object_destroy_packet);
    }
    
    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        RemoveObject(object_id);
    }
}

void Map::DestroyDroppedItem(uint32_t object_id, uint32_t character_id)
{
    ObjectDestroyInfo info;
    info.type = ObjectType::kDroppedItem;
    info.object_id = object_id;
    info.info.dropped_item.character_id = character_id;
    
    ObjectDestroyPacket object_destroy_packet;
    object_destroy_packet.object_info = info;
    
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        SendPacket(object_destroy_packet);
    }
    
    {
        std::lock_guard<std::mutex> lock(object_mutex_);
        RemoveObject(object_id);
    }
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
    
    int32_t value = 0;
    
    {
        auto it = players_.find(attacker);
        if (it != players_.end())
        {
            auto player = it->second.lock();
            if (player) value = player->GetBuffedValue(BuffStat::kAtk);
        }
    }

    auto it = map_objects_.find(defender);
    if (it != map_objects_.end())
    {
        Mob* mob = dynamic_cast<Mob*>(it->second.get());
        if (mob)
        {
            mob->TakeDamage(attacker, 1000 + value);
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

void Map::GetOverlappingObjects(const Bounds& bounds, std::vector<std::shared_ptr<MapObject>>& result)
{
    std::lock_guard<std::mutex> lock(object_mutex_);
     
    for (const auto& [id, obj] : map_objects_)
    {
        // 임시 사이즈
        Bounds target_bounds(obj->GetPosition(), {3.f, 2.f});
        Bounds intersect_bounds = Bounds::Intersect(bounds, target_bounds);

        if (intersect_bounds.size.x >= 0 && intersect_bounds.size.y >= 0)
            result.push_back(obj);
    }
}

void Map::GetDropPosition(Math::Vector2& position) const
{
    position.y += 2.f;
    position.x = Math::Clamp(position.x, map_bounds_.min.x, map_bounds_.max.x);

    Foothold* foothold = FindFoothold(position);
    if (foothold) position.y = foothold->GetYAt(position.x);
}

bool Map::LoadMapData()
{
    std::string path = std::format(".\\Content\\Tilemaps\\{:06}.tmx", map_id_);
        
    tmx::Map map_data;
    if (!map_data.load(path)) return false;

    const auto& properties = map_data.getProperties();
    if (properties.empty()) return false;

    float ppu = properties[2].getFloatValue();
    return_map_id_ = properties[3].getIntValue();

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
                        const auto& object_properties = object.getProperties();
                        if (object_properties.empty()) continue;

                        int32_t id = object_properties[0].getIntValue();
                        int32_t next = object_properties[1].getIntValue();
                        int32_t previous = object_properties[2].getIntValue();
                        
                        const auto& points = object.getPoints();
                        Math::Vector2 point1 = {
                            points[0].x / ppu + object.getPosition().x / ppu - map_data.getTileCount().x / 2.f,
                            -1 * points[0].y / ppu - object.getPosition().y / ppu + map_data.getTileCount().y / 2.f
                        };
                            
                        Math::Vector2 point2 = {
                            points[1].x / ppu + object.getPosition().x / ppu - map_data.getTileCount().x / 2.f,
                            -1 * points[1].y / ppu - object.getPosition().y / ppu + map_data.getTileCount().y / 2.f
                        };
                        
                        footholds_.insert_or_assign(id, std::make_unique<Foothold>(point1, point2, id, previous, next));
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

                    uint32_t mob_id = properties[0].getIntValue();
                    spawn_points_.emplace_back(std::make_unique<SpawnPoint>(mob_id, position));
                }
            }
            else if (layer->getName() == "Portal")
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

                    int32_t id = properties[0].getIntValue();
                    int32_t to_id = properties[1].getIntValue();
                    int32_t to_map = properties[2].getIntValue();

                    portals_.insert_or_assign(id, std::make_unique<Portal>(id, to_id, to_map, position));
                }
            }
        }
    }

    // 맵에 배치된 블럭 정보 조회
    sql::Connection* connection = MySQLManager::Get()->GetConnection();
    if (!connection) return false;

    try
    {
        std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("SELECT * FROM block_info WHERE map_id = ?"));
        statement->setUInt(1, map_id_);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
        while (result->next())
        {
            std::wstring color = StringHelper::UTF8ToUTF16(result->getString("color"));
            int32_t hp = result->getInt("hp");
            
            Math::Vector2 position;
            position.x = static_cast<float>(result->getDouble("position_x"));
            position.y = static_cast<float>(result->getDouble("position_y"));
            
            SpawnBlock(color, hp, position);
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
    }

    return true;
}

std::shared_ptr<MapObject> Map::FindMapObject(uint32_t object_id)
{
    auto it = map_objects_.find(object_id);
    if (it == map_objects_.end()) return nullptr;
    return it->second;
}

Foothold* Map::FindFoothold(const Math::Vector2& position) const
{
    Foothold* best = nullptr;
    float best_y = map_bounds_.min.y;

    for (const auto& it : footholds_)
    {
        Foothold* foothold = it.second.get();
        if (position.x < foothold->GetX1() || position.x > foothold->GetX2()) continue;
        
        float y = foothold->GetYAt(position.x);
        if (y <= position.y && y > best_y)
        {
            best_y = y;
            best = foothold;
        }
    }
    
    return best;
}

Foothold* Map::FindFootholdByID(int32_t foothold_id)
{
    auto it = footholds_.find(foothold_id);
    if (it == footholds_.end()) return nullptr;
    return it->second.get();
}

Portal* Map::FindPortal(int32_t portal_id)
{
    auto it = portals_.find(portal_id);
    if (it == portals_.end()) return nullptr;
    return it->second.get();
}

std::shared_ptr<PlayerCharacter> Map::FindPlayer(uint32_t player_id)
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
    
    int32_t max_spawnable_mobs = spawn_points_.size() - number_spawned_mobs_.load();
    if (max_spawnable_mobs <= 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::vector<std::shared_ptr<SpawnPoint>> spawn_points = spawn_points_;
    std::ranges::shuffle(spawn_points, gen);

    int32_t number_spawned = 0;
    for (const auto& spawn_point : spawn_points)
    {
        if (const MobData* mob_data = DataManager::Get()->GetMob(spawn_point->GetMobID()))
        {
            std::shared_ptr<Mob> mob = std::make_shared<Mob>(*mob_data);
            mob->SetPosition(spawn_point->GetPosition());
            mob->SetLastPosition(spawn_point->GetPosition());
            SpawnMob(mob);

            if (++number_spawned >= max_spawnable_mobs) break;
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
            uint32_t object_id = mob->GetObjectID();
            
            ObjectDestroyInfo info;
            info.type = ObjectType::kMob;
            info.object_id = object_id;
    
            ObjectDestroyPacket object_destroy_packet;
            object_destroy_packet.object_info = info;
    
            {
                // std::lock_guard<std::mutex> player_lock(player_mutex_);
                SendPacket(object_destroy_packet);
            }
            
            RemoveObject(object_id);
        }
    }

    number_spawned_mobs_.store(0);
}

void Map::OnMobDeath(const std::shared_ptr<Mob>& mob)
{
    if (const auto* drops = DataManager::Get()->GetDrop(mob->GetMobID()))
    {
        int32_t d = 0;
        for (const auto& drop : *drops)
        {
            int32_t drop_chance = Math::RandRange(0, 9999); // 0.01%
            if (drop_chance > drop.chance) continue;
                
            int32_t count = Math::RandRange(drop.min_count, drop.max_count);
            if (count <= 0) continue;
                
            int32_t step = (d + 1) / 2;
            int32_t sign = (d % 2) ? 1 : -1;

            Math::Vector2 drop_position = mob->GetPosition();
            drop_position.x += static_cast<float>(sign * step) * .5f;
            GetDropPosition(drop_position);

            if (drop.id == 0)
                SpawnColorDrop(count, mob, drop_position);
            else
            {
                uint32_t type_index = drop.id / 100000;
                if (type_index == 1) SpawnItemDrop(EquipItem::Create(drop.id), mob, drop_position);
                else SpawnItemDrop(Item::Create(drop.id, count), mob, drop_position);
            }
                
            ++d;
        }
    }
    
    number_spawned_mobs_.fetch_sub(1);
    DestroyMob(mob->GetObjectID());
}
