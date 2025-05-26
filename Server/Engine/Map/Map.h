#pragma once
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>
#include "rttr/type.h"
#include "Actor/Actor.h"

namespace Net
{
    struct IPacket;
}

class Player;
class Actor;

class Map
{
public:
    Map(uint32_t MapBase_id);
    virtual ~Map();

    virtual void Init();
    
    virtual void AddPlayer(const std::weak_ptr<Player> &player_weak);
    virtual void RemovePlayer(uint32_t player_id);
    virtual void AddPlayers();
    virtual void RemovePlayers();
    
    virtual void SendPacket(const Net::IPacket& packet);
    virtual void SendPacket(const Net::IPacket& packet, const std::weak_ptr<Player> &excluded_player_weak);

    virtual void Tick(float delta_time);
    virtual void PhysicsTick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void ActivateActor(Actor* actor, bool is_active);
    virtual void DestroyActor(Actor* actor);
    virtual void ProcessTriggerEvents();

    bool LoadMapData();
    
    void ProcessCollisionEvents();

    virtual void InitPhysicsWorld();
    
    inline size_t GetPlayerCount() const { return players_.size(); }
    std::vector<std::weak_ptr<Player>> GetPlayers();
    inline uint32_t GetMapBaseUniqueID() const { return map_unique_id_; }

    b2WorldId GetWorldID() const {return world_id_; }
    
    template <std::derived_from<Actor> T>
    std::shared_ptr<T> SpawnActor(const rttr::type& kType, const std::wstring& kName = L"");
private:
    struct ActorActivation
    {
        Actor* actor;
        bool is_active;
    };
    void TransitionLevel();
    void ProcessActorActivation();
    void SpawnActors();
    void DestroyActors();

    std::mutex player_mutex_;
    uint32_t map_unique_id_;
    b2WorldId world_id_;
	uint32_t test_next_unique_id_;

    std::unordered_map<uint32_t, std::shared_ptr<Actor>> map_objects_;
    std::unordered_map<uint32_t, std::weak_ptr<Player>> players_;

    std::vector<std::array<Math::Vector2, 4>> collider_vertices_;

    std::queue<std::weak_ptr<Player>> pending_players_;
    std::queue<uint32_t> pending_remove_players_;
    std::queue<std::shared_ptr<Actor>> pending_actors_;
    std::queue<std::shared_ptr<Actor>> pending_destroy_actors_;
    std::queue<ActorActivation> pending_actor_activation_;
};

template <std::derived_from<Actor> T>
std::shared_ptr<T> Map::SpawnActor(const rttr::type& kType, const std::wstring& kName)
{
    std::wstring name = kName;
    
    if (name.empty())
    {
        std::string type_name = kType.get_name().to_string();
        name.assign(type_name.begin(), type_name.end());
    }
    
    rttr::variant var = kType.create({ name });
    if (var.is_valid())
    {
        std::shared_ptr<Actor> actor = var.get_value<std::shared_ptr<Actor>>();
        pending_actors_.push(actor);

        actor->SetMap(this);
		actor->SetUniqueID(test_next_unique_id_++);
        actor->InitializeActor();

        rttr::type actor_type = rttr::type::get<T>();
        if (kType.is_derived_from(actor_type))
        {
            return std::static_pointer_cast<T>(actor);
        }
    }

    return nullptr;
}

FORCEINLINE bool IsValid(const Map* map)
{
    return map != nullptr;
}