#include "pch.h"
#include "Map.h"

#include <CustomPacket.h>
#include <format>
#include <ranges>

#include "Session/Player.h"
#include "tmxlite/Map.hpp"

Map::Map(uint32_t map_id) :
    map_id_(map_id),
    test_next_unique_id_(1000),
    collider_polygons_()
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

        uint32_t unique_id = player->GetCharacterID();
        players_.emplace(unique_id, pending_player_weak);
    
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
        for (auto & [unique_key, other_player_weak] : players_)
        {
            auto other_player = other_player_weak.lock();
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

void Map::Tick(float delta_time)
{
    AddPlayers();
    RemovePlayers();
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

            if (layer->getName() == "Collision")
            {
                const auto& objects = object_group.getObjects();
                for (const auto& object : objects)
                {
                    if (object.getShape() == tmx::Object::Shape::Rectangle)
                    {
                        float half_width = object.getAABB().width / 2.f / ppu;
                        float half_height = object.getAABB().height / 2.f / ppu;
                        
                        Math::Vector2 center = {
                            object.getPosition().x / ppu + half_width - map_data.getTileCount().x / 2.f,
                            -1 * object.getPosition().y / ppu - half_height + map_data.getTileCount().y / 2.f
                        };

                        Collider::Polygon polygon;
                        polygon.vertices.emplace_back(center.x - half_width, center.y - half_height);
                        polygon.vertices.emplace_back(center.x + half_width, center.y - half_height);
                        polygon.vertices.emplace_back(center.x + half_width, center.y + half_height);
                        polygon.vertices.emplace_back(center.x - half_width, center.y + half_height);
                        collider_polygons_.push_back(polygon);
                    }
                    else if (object.getShape() == tmx::Object::Shape::Polygon)
                    {
                        Collider::Polygon polygon;
                        
                        const auto& points = object.getPoints();
                        for (int32_t i = 0; i < points.size(); ++i)
                        {
                            polygon.vertices.emplace_back(
                                points[(i + 1) % points.size()].x / ppu + object.getPosition().x / ppu - map_data.getTileCount().x / 2.f,
                                -1 * points[(i + 1) % points.size()].y / ppu - object.getPosition().y / ppu + map_data.getTileCount().y / 2.f
                            );
                        }

                        collider_polygons_.push_back(polygon);
                    }
                }
                
                break;
            }
        }
    }

    return true;
}
