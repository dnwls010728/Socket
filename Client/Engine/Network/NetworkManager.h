#pragma once
#include "Singleton.h"

class NetworkActor;

class NetworkManager : public Singleton<NetworkManager>
{
public:
    NetworkManager();
    virtual ~NetworkManager() override = default;

    FORCEINLINE void SetPlayerClass(const rttr::type& kPlayerClass) { player_class_ = kPlayerClass; }
    
    FORCEINLINE int GetPacketId() const { return packet_id_; }

private:
    friend class World;
    
    void Tick(float delta_time);

    rttr::type player_class_;

    int packet_id_;

    std::map<uint32_t, NetworkActor*> players_;
    
};
