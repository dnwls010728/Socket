#pragma once
#include "Singleton.h"
#include "Level/World.h"

class NetworkActor;

class NetworkManager : public Singleton<NetworkManager>
{
public:
    NetworkManager();
    virtual ~NetworkManager() override = default;
    
    template<std::derived_from<NetworkActor> T>
    T* SpawnActor(const rttr::type& kType, const std::wstring& kName = L"");

    FORCEINLINE void SetPlayerClass(const rttr::type& kPlayerClass) { player_class_ = kPlayerClass; }
    
    FORCEINLINE int GetPacketId() const { return packet_id_; }

private:
    friend class World;
    
    void Tick(float delta_time);

    rttr::type player_class_;

    int packet_id_;

    std::map<uint32_t, NetworkActor*> players_;
    
};

template <std::derived_from<NetworkActor> T>
T* NetworkManager::SpawnActor(const rttr::type& kType, const std::wstring& kName)
{
    return World::Get()->SpawnActor<T*>(kType, kName);
}
