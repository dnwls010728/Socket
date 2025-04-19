#pragma once
#include <cstdint>

namespace Net
{
    struct IPacket;
}

class Session;

class Player
{
public:
    Player(Session* session, uint32_t account_unique_id);
    ~Player() = default;

    void SendPacket(const Net::IPacket& packet);

    inline Session* GetSession() const { return session_; }
    inline uint32_t GetAccountUniqueID() const { return account_unique_id_; }

private:
    Session* session_;

    uint32_t account_unique_id_;
    
};
