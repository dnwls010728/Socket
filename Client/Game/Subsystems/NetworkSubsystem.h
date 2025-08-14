#pragma once
#include "Subsystem/Tickable.h"
#include "Subsystem/WorldSubsystem.h"

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

    virtual void Tick(float delta_time) override;

    void SendPacket(Net::IPacket& packet);
    void ChangeMap(int32_t map_id);
    void RegisterNetworkActor(const std::shared_ptr<NetworkActor>& actor);
    void UnregisterNetworkActor(const std::shared_ptr<NetworkActor>& actor);
    
    template <std::derived_from<NetworkActor> T>
    std::shared_ptr<T> SpawnNetworkActor(const rttr::type& type, uint32_t unique_id, const std::wstring& name = L"");
    
    void DestroyNetworkActor(uint32_t unique_id);
    void SetPlayerCharacter(const std::shared_ptr<PlayerCharacter>& player);
    void GetOtherPlayers(std::vector<std::shared_ptr<PlayerCharacter>>& out_players);

    std::shared_ptr<NetworkActor> FindNetworkActor(uint32_t unique_id);
    
    FORCEINLINE const std::unordered_map<uint32_t, std::shared_ptr<NetworkActor>>& GetNetworkActors() { return network_actors_; }
    FORCEINLINE std::shared_ptr<PlayerCharacter> GetPlayer() const { return player_.lock(); }

    static NetworkSubsystem* Get();

private:
    friend class MapSetupHandler;
    friend class ChangeMapHandler;
    friend class ObjectSpawnHandler;

    std::unordered_map<uint32_t, std::shared_ptr<NetworkActor>> network_actors_;

    std::weak_ptr<PlayerCharacter> player_;
    std::vector<std::weak_ptr<PlayerCharacter>> other_players_;
    
};

template <std::derived_from<NetworkActor> T>
std::shared_ptr<T> NetworkSubsystem::SpawnNetworkActor(const rttr::type& type, uint32_t unique_id, const std::wstring& name)
{
    std::shared_ptr<T> network_actor = World::Get()->SpawnActor<T>(type, name);
    if (IsValid(network_actor))
    {
        network_actors_.emplace(unique_id, network_actor);
        network_actor->SetObjectID(unique_id);
        return network_actor;
    }

    return nullptr;
}
