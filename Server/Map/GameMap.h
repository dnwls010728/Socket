#pragma once
#include <cstdint>
#include <vector>

namespace Net
{
    struct IPacket;
}

class Player;

class GameMap
{
public:
    GameMap(uint32_t map_id);
    ~GameMap() = default;

    void AddPlayer(Player* player);
    void RemovePlayer(Player* player);
    void SendPacket(const Net::IPacket& packet);
    void SendPacket(const Net::IPacket& packet, Player* excluded_player);

    inline uint32_t GetMapUniqueID() const { return map_unique_id_; }

private:
    uint32_t map_unique_id_;

    std::vector<Player*> players_;
    
};
