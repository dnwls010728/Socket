#pragma once
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

#include "MapObjects/Foothold.h"
#include "Math/Vector2.h"

class Mob;

namespace Net
{
    struct IPacket;
}

class Player;
class MapObject;

class Map
{
public:
    Map(uint32_t map_id);
    ~Map() = default;

    void AddPlayer(const std::weak_ptr<Player>& player_weak);
    void RemovePlayer(uint32_t player_id);
    void AddPlayers();
    void RemovePlayers();
    
    void AddObject(const std::shared_ptr<MapObject>& object);
    void RemoveObject(uint32_t object_id);
    void SpawnObject(const std::shared_ptr<MapObject>& object);

    void SendPacket(const Net::IPacket& packet);
    void SendPacket(const Net::IPacket& packet, const std::weak_ptr<Player>& excluded_player_weak);

    void Tick(float delta_time);

    bool LoadMapData();

    Foothold* FindFoothold(const Math::Vector2& position);

    std::vector<std::weak_ptr<Player>> GetPlayers();

    inline size_t GetPlayerCount() const { return players_.size(); }
    inline uint32_t GetMapID() const { return map_id_; }

private:
    void AddObjects();
    void RemoveObjects();

    uint32_t map_id_;

    std::mutex player_mutex_;
    std::mutex object_mutex_;

    std::atomic<uint32_t> next_object_id_;

    std::unordered_map<uint32_t, std::weak_ptr<Player>> players_;

    std::map<uint32_t, std::shared_ptr<MapObject>> map_objects_;

    std::queue<std::weak_ptr<Player>> pending_players_;
    std::queue<uint32_t> pending_remove_players_;

    std::queue<std::shared_ptr<MapObject>> pending_objects_;
    std::queue<uint32_t> pending_remove_objects_;

    std::vector<std::unique_ptr<Foothold>> footholds_;
};
