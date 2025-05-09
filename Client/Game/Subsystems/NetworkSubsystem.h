#pragma once
#include "Subsystems/Tickable.h"
#include "Subsystems/WorldSubsystem.h"

#define GET_NETWORK() World::Get()->GetSubsystem<NetworkSubsystem>()

class Tilemap;
class PlayerCharacter;
class NetworkActor;

namespace Net
{
    struct IPacket;
}

class NetworkSubsystem : public WorldSubsystem, public Tickable
{
    GENERATED_BODY(NetworkSubsystem, WorldSubsystem)
    
public:
    NetworkSubsystem();
    virtual ~NetworkSubsystem() override = default;

    virtual void Init() override;
    virtual void Deinit() override;
    virtual void OnWorldBeginPlay() override;
    virtual void Tick(float delta_time) override;

    void SendPacket(Net::IPacket& packet);
    void ChangeMap(uint32_t map_unique_id);
    
    template <std::derived_from<NetworkActor> T>
    std::shared_ptr<T> SpawnNetworkActor(const rttr::type& type, uint32_t unique_id, const std::wstring& name = L"");
    
    void DestroyNetworkActor(uint32_t unique_id);
    void GetOtherPlayers(std::vector<std::shared_ptr<PlayerCharacter>>& out_players);

    std::shared_ptr<NetworkActor> GetNetworkActor(uint32_t unique_id);

    FORCEINLINE const std::unordered_map<uint32_t, std::shared_ptr<NetworkActor>>& GetNetworkActors() { return network_actors_; }
    FORCEINLINE std::shared_ptr<PlayerCharacter> GetPlayer() const { return player_.lock(); }

    FORCEINLINE Tilemap* GetTilemap() const { return tilemap_; }

private:
    void ProcessPackets(const std::shared_ptr<Net::IPacket>& packet);
    void TransitionMap(uint32_t map_unique_id);

    std::unordered_map<uint32_t, std::shared_ptr<NetworkActor>> network_actors_;

    std::weak_ptr<PlayerCharacter> player_;
    std::vector<std::weak_ptr<PlayerCharacter>> other_players_;

    Tilemap* tilemap_;
    
};

template <std::derived_from<NetworkActor> T>
std::shared_ptr<T> NetworkSubsystem::SpawnNetworkActor(const rttr::type& type, uint32_t unique_id, const std::wstring& name)
{
    std::shared_ptr<T> network_actor = World::Get()->SpawnActor<T>(type, name);
    if (IsValid(network_actor))
    {
        network_actors_.emplace(unique_id, network_actor);
        network_actor->SetUniqueID(unique_id);
        return network_actor;
    }

    return nullptr;
}
