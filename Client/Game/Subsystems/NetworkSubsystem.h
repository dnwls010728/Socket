#pragma once
#include "Subsystems/Tickable.h"
#include "Subsystems/WorldSubsystem.h"

#define GET_NETWORK() World::Get()->GetSubsystem<NetworkSubsystem>()

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
    
    template <std::derived_from<NetworkActor> T>
    std::shared_ptr<T> SpawnNetworkActor(const rttr::type& kType, const std::wstring& kName, Type::uint32 unique_id);
    
    void DestroyNetworkActor(Type::uint32 unique_id);

    std::shared_ptr<NetworkActor> GetNetworkActor(Type::uint32 unique_id);

private:
    void ProcessPackets(const std::shared_ptr<Net::IPacket>& packet);

    std::unordered_map<Type::uint32, std::shared_ptr<NetworkActor>> network_actors_;
    
};

template <std::derived_from<NetworkActor> T>
std::shared_ptr<T> NetworkSubsystem::SpawnNetworkActor(const rttr::type& kType, const std::wstring& kName, Type::uint32 unique_id)
{
    std::shared_ptr<T> network_actor = World::Get()->SpawnActor<T>(kType, kName);
    if (IsValid(network_actor))
    {
        network_actors_.emplace(unique_id, network_actor);
        network_actor->SetUniqueID(unique_id);
        return network_actor;
    }

    return nullptr;
}
