#pragma once
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>

class MapObject;

namespace Net
{
    struct IPacket;
}

class Player;

class Map
{
public:
    Map(uint32_t map_id);
    ~Map() = default;

    void AddPlayer(Player* player);
    void RemovePlayer(Player* player);
    void SendPacket(const Net::IPacket& packet);
    void SendPacket(const Net::IPacket& packet, const Player* excluded_player);

    uint32_t GetMapObjectUniqueID();

    inline uint32_t GetMapUniqueID() const { return map_unique_id_; }

private:
    std::mutex mutex_;
    
    uint32_t map_unique_id_;
    
    std::atomic<uint32_t> map_object_unique_id_;

    std::unordered_map<uint32_t, std::shared_ptr<MapObject>> map_objects_;
    
    std::vector<Player*> players_;
    
};
