#pragma once
#include <cstdint>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

#include "Foothold.h"
#include "Portal.h"
#include "Math/Bounds.h"
#include "Math/Vector2.h"

class NPC;
class Item;
class SpawnPoint;
class PlayerCharacter;

namespace Net
{
    struct IPacket;
}

class MapObject;
class Mob;
class ProjectileObject;

class Map
{
public:
    Map(uint32_t map_id);
    ~Map() = default;

    void AddPlayer(const std::weak_ptr<PlayerCharacter>& player_weak);
    void RemovePlayer(uint32_t player_id);
    void AddPlayers();
    void RemovePlayers();
    
    void AddObject(const std::shared_ptr<MapObject>& object);
    void RemoveObject(uint32_t object_id);
    void SpawnMob(const std::shared_ptr<MapObject>& object);
    void SpawnProjectile(const std::shared_ptr<ProjectileObject>& projectile);
    void SpawnColorDrop(int32_t color, const std::shared_ptr<MapObject>& dropper, const Math::Vector2& drop_position);
    void SpawnItemDrop(const std::shared_ptr<Item>& item, const std::shared_ptr<MapObject>& dropper, const Math::Vector2& drop_position);

    void DestroyMob(uint32_t object_id);
    void DestroyProjectile(uint32_t object_id);
    void DestroyDroppedItem(uint32_t object_id, uint32_t character_id);

    void SendPacket(const Net::IPacket& packet);
    void SendPacket(const Net::IPacket& packet, const std::weak_ptr<PlayerCharacter>& excluded_player_weak);

    void PhysicsTick(float delta_time);
    void Tick(float delta_time);

    bool LoadMapData();
    
    std::shared_ptr<MapObject> FindMapObject(uint32_t object_id);

    Foothold* FindFoothold(const Math::Vector2& position) const;
    Foothold* FindFootholdByID(int32_t foothold_id);

    Portal* FindPortal(int32_t portal_id);

    std::shared_ptr<PlayerCharacter> FindPlayer(uint32_t player_id);

    std::vector<std::weak_ptr<PlayerCharacter>> GetPlayers();

    void GetDropPosition(Math::Vector2& position) const;

    void GetOverlappingObjects(const Bounds& bounds, std::vector<std::shared_ptr<MapObject>>& result);

    inline size_t GetPlayerCount() const { return players_.size(); }
    inline uint32_t GetMapID() const { return map_id_; }
    inline uint32_t GetReturnMapID() const { return return_map_id_; }
    inline const Bounds& GetMapBounds() const { return map_bounds_; }

private:
    void AddObjects();
    void RemoveObjects();
    void Respawn();
    void KillAllMobs();
    
    void OnMobDeath(const std::shared_ptr<Mob>& mob);

    uint32_t map_id_;
    uint32_t return_map_id_;

    Bounds map_bounds_;

    std::mutex player_mutex_;
    std::mutex object_mutex_;

    std::atomic_uint32_t next_object_id_;
    
    std::atomic_int32_t number_spawned_mobs_;

    std::unordered_map<int32_t, std::weak_ptr<PlayerCharacter>> players_;
    std::unordered_map<int32_t, std::unique_ptr<Foothold>> footholds_;
    std::unordered_map<int32_t, std::unique_ptr<Portal>> portals_;

    std::map<uint32_t, std::shared_ptr<MapObject>> map_objects_;

    std::queue<std::weak_ptr<PlayerCharacter>> pending_players_;
    std::queue<uint32_t> pending_remove_players_;

    std::queue<std::shared_ptr<MapObject>> pending_objects_;
    std::queue<uint32_t> pending_remove_objects_;

    std::vector<uint32_t> mob_ids;

    float respawn_timer_;
    float monitor_timer_;

    std::vector<std::shared_ptr<SpawnPoint>> spawn_points_;
};
