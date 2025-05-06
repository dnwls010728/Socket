#pragma once
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

class MapObject;
struct ActorActivation
{
    MapObject* actor;
    bool is_active;
};

namespace Net
{
    struct IPacket;
}

class Player;

class MapBase
{
public:
    MapBase(uint32_t MapBase_id);
    ~MapBase() = default;

    virtual void AddPlayer(Player* player);
    virtual void RemovePlayer(Player* player);
    virtual void SendPacket(const Net::IPacket& packet);
    virtual void SendPacket(const Net::IPacket& packet, const Player* excluded_player);

    virtual void Tick(float delta_time);
    virtual void Physics(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void ActivateActor(MapObject* actor, bool is_active);
    virtual void DestroyActor(MapObject* actor);
    
    inline size_t GetPlayerCount() const { return players_.size(); }
    inline uint32_t GetMapBaseUniqueID() const { return map_unique_id_; }
private:
    void TransitionLevel();
    void ProcessActorActivation();
    void SpawnActors();
    void DestroyActors();

    std::mutex mutex_;
    uint32_t map_unique_id_;

    std::unordered_map<uint32_t, std::shared_ptr<MapObject>> map_objects_;
    std::vector<Player*> players_;
    
    std::queue<std::shared_ptr<MapObject>> pending_actors_;
    std::queue<std::shared_ptr<MapObject>> pending_destroy_actors_;
    std::queue<ActorActivation> pending_actor_activation_;
};
