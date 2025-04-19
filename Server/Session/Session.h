#pragma once
#include <memory>

namespace Net
{
    struct IPacket;
}

class Player;

class Session
{
public:
    Session(int client_id);
    ~Session() = default;

    void SendPacket(const Net::IPacket& packet);

    std::shared_ptr<Player> CreatePlayer(uint32_t account_unique_id);

    uint32_t GetAccountUniqueID() const;

    inline int GetClientID() const { return client_id_; }
    
    inline std::shared_ptr<Player> GetPlayer() const { return player_; }

private:
    int client_id_;

    std::shared_ptr<Player> player_;
};
