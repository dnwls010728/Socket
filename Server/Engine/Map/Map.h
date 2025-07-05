#pragma once
#include <cstdint>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

#include "Foothold.h"
#include "Math/Vector2.h"

namespace Net
{
    struct IPacket;
}

struct SpawnPoint
{
    Math::Vector2 position;
    uint32_t mob_id;
};

class Player;
class MapObject;
class Mob;

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
    void DestroyObject(uint32_t object_id);

    void SendPacket(const Net::IPacket& packet);
    void SendPacket(const Net::IPacket& packet, const std::weak_ptr<Player>& excluded_player_weak);

    void OnAttack(uint32_t attacker_id, uint32_t defender_id);

    void PhysicsTick(float delta_time);
    void Tick(float delta_time);

    bool LoadMapData();

    Foothold* FindFoothold(const Math::Vector2& position);

    std::vector<std::weak_ptr<Player>> GetPlayers();

    inline size_t GetPlayerCount() const { return players_.size(); }
    inline uint32_t GetMapID() const { return map_id_; }

private:
    void AddObjects();
    void RemoveObjects();
    void Respawn();
    void KillAllMobs();
    void DestroyObject_Internal(uint32_t object_id);

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
    std::vector<uint32_t> mob_ids;

    float respawn_timer_;
    float monitor_timer_;

    std::vector<SpawnPoint> spawn_points_;
};
