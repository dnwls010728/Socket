#pragma once
#include <memory>

#include "Player.h"

namespace Net
{
    struct IPacket;
}

class Session
{
public:
    enum class State
    {
        kNone,
        kConnected,
        kDisconnected,
        kLoggedIn,
        kCharacterSelected,
        kInGame
    };
    
    Session(int client_id);
    ~Session() = default;

    void SendPacket(const Net::IPacket& packet) const;
    void ReceivePacket(Net::IPacket* packet) const;

    std::shared_ptr<Player> CreatePlayer(uint32_t account_unique_id);

    uint32_t GetAccountUniqueID() const;

    inline int GetClientID() const { return client_id_; }

    inline State GetState() const { return state_; }
    inline void SetState(const State state) { state_ = state; }
    
    inline std::shared_ptr<Player> GetPlayer() const { return player_; }

private:
    int client_id_;

    State state_;

    std::shared_ptr<Player> player_;
};
