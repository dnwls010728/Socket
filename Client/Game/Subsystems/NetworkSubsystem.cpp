#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>
#include <ranges>

#include "DebugDrawHelper.h"
#include "GameInstance.h"
#include "SessionSubsystem.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actors/TilemapLoader.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Mobs/MobBase.h"
#include "Asset/AssetManager.h"
#include "imgui/imgui.h"
#include "Input/Keyboard.h"

NetworkSubsystem::NetworkSubsystem() :
    network_actors_(),
    player_(),
    other_players_()
{
}

void NetworkSubsystem::Tick(float delta_time)
{
    Tickable::Tick(delta_time);

    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem)
    {
        session_subsystem->ProcessPackets();
    }
}

void NetworkSubsystem::SendPacket(Net::IPacket& packet)
{
    SessionSubsystem::Get()->SendPacket(packet);
}

void NetworkSubsystem::ChangeMap(int32_t map_id)
{
    ChangeMapPacket request;
    request.map_id = map_id;
    SendPacket(request);
}

void NetworkSubsystem::RegisterNetworkActor(const std::shared_ptr<NetworkActor>& actor)
{
    if (!IsValid(actor)) return;
    
    uint32_t object_id = actor->GetObjectID();
    if (!network_actors_.contains(object_id)) network_actors_[object_id] = actor;
}

void NetworkSubsystem::UnregisterNetworkActor(const std::shared_ptr<NetworkActor>& actor)
{
    if (!IsValid(actor)) return;
    
    uint32_t object_id = actor->GetObjectID();
    
    auto iter = network_actors_.find(object_id);
    if (iter != network_actors_.end()) network_actors_.erase(iter);
}

void NetworkSubsystem::DestroyNetworkActor(uint32_t unique_id)
{
    auto iter = network_actors_.find(unique_id);
    if (iter != network_actors_.end())
    {
        std::shared_ptr<NetworkActor> network_actor = iter->second;
        if (IsValid(network_actor)) network_actor->Destroy();
        network_actors_.erase(iter);
    }
}

void NetworkSubsystem::SetPlayerCharacter(const std::shared_ptr<PlayerCharacter>& player)
{
    if (IsValid(player))
    {
        player->SetMine(true);
        
        player_ = player;
        network_actors_[player->GetObjectID()] = player;
    }
}

void NetworkSubsystem::GetOtherPlayers(std::vector<std::shared_ptr<PlayerCharacter>>& out_players)
{
    for (const auto& player : other_players_)
    {
        std::shared_ptr<PlayerCharacter> player_character = player.lock();
        if (IsValid(player_character))
        {
            out_players.push_back(player_character);
        }
    }
}

std::shared_ptr<NetworkActor> NetworkSubsystem::FindNetworkActor(const uint32_t unique_id)
{
    auto iter = network_actors_.find(unique_id);
    if (iter != network_actors_.end()) return iter->second;
    return nullptr;
}

NetworkSubsystem* NetworkSubsystem::Get()
{
    return World::Get()->GetSubsystem<NetworkSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<NetworkSubsystem>("NetworkSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
