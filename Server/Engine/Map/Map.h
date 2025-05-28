#pragma once
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <queue>

#include "Math/Vector2.h"

namespace Net
{
    struct IPacket;
}

class Player;
class Actor;

struct Foothold
{
    Math::Vector2 point1;
    Math::Vector2 point2;

    inline bool IsValid() const { return point1 != point2; }
};

class Map
{
public:
    Map(uint32_t map_id);
    virtual ~Map() = default;
    
    virtual void AddPlayer(const std::weak_ptr<Player> &player_weak);
    virtual void RemovePlayer(uint32_t player_id);
    virtual void AddPlayers();
    virtual void RemovePlayers();
    
    virtual void SendPacket(const Net::IPacket& packet);
    virtual void SendPacket(const Net::IPacket& packet, const std::weak_ptr<Player> &excluded_player_weak);

    virtual void Tick(float delta_time);

    bool LoadMapData();

    Foothold GetFoothold(const Math::Vector2& position);
    
    float GetFootholdY(const Foothold& foothold, const Math::Vector2& position);
    
    std::vector<std::weak_ptr<Player>> GetPlayers();
    
    inline size_t GetPlayerCount() const { return players_.size(); }
    inline uint32_t GetMapID() const { return map_id_; }

private:
    std::mutex player_mutex_;
    uint32_t map_id_;
	uint32_t test_next_unique_id_;

    std::unordered_map<uint32_t, std::weak_ptr<Player>> players_;

    std::vector<Foothold> footholds_;

    std::queue<std::weak_ptr<Player>> pending_players_;
    std::queue<uint32_t> pending_remove_players_;
    
    static bool CompareToFoothold(const Foothold& a, const Foothold& b);
};

FORCEINLINE bool IsValid(const Map* map)
{
    return map != nullptr;
}