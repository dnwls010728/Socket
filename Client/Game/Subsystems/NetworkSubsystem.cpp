#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "SessionSubsystem.h"
#include "Actor/Component/TransformComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Level/CameraManager.h"

NetworkSubsystem::NetworkSubsystem() :
    network_actors_()
{
}

void NetworkSubsystem::Init()
{
    WorldSubsystem::Init();
    
}

void NetworkSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    GET_SESSION()->packet_handler.Remove(this, &NetworkSubsystem::ProcessPackets);
}

void NetworkSubsystem::OnWorldBeginPlay()
{
    WorldSubsystem::OnWorldBeginPlay();
    
    GET_SESSION()->packet_handler.Add(this, &NetworkSubsystem::ProcessPackets);

    const CharacterInfo& character_info = GET_SESSION()->GetCharacterInfo();
    if (GET_SESSION()->IsInGame())
    {
        std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), L"PlayerCharacter", character_info.unique_id);
        if (IsValid(player_character))
        {
            player_character->SetMine(true);

            CameraManager* camera_manager = CameraManager::Get();
            camera_manager->SetSize(6.f);
            camera_manager->SetTickType(TickType::kTick);
            camera_manager->SetTarget(player_character);
        }

        MapLoadCompletePacket map_load_complete_packet;
        GET_SESSION()->SendPacket(map_load_complete_packet);
    }
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
    GET_SESSION()->SendPacket(packet);
}

void NetworkSubsystem::OpenLevel(uint32_t map_unique_id)
{
    ChangeMapRequest request;
    request.map_unique_id = map_unique_id;
    SendPacket(request);
}

void NetworkSubsystem::DestroyNetworkActor(Type::uint32 unique_id)
{
    auto iter = network_actors_.find(unique_id);
    if (iter != network_actors_.end())
    {
        std::shared_ptr<NetworkActor> network_actor = iter->second;
        if (IsValid(network_actor)) network_actor->Destroy();
        network_actors_.erase(iter);
    }
}

std::shared_ptr<NetworkActor> NetworkSubsystem::GetNetworkActor(const Type::uint32 unique_id)
{
    auto iter = network_actors_.find(unique_id);
    if (iter != network_actors_.end()) return iter->second;
    return nullptr;
}

void NetworkSubsystem::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
{
    switch (packet->GetPacketID())
    {
    case ChangeMapResponse::StaticPacketID:
        {
            ChangeMapResponse* response = static_cast<ChangeMapResponse*>(packet.get());
            if (response->is_success)
            {
                World::Get()->OpenLevel(std::to_wstring(response->map_unique_id));
            }
        }
        break;
        
    case SpawnPlayerPacket::StaticPacketID:
        {
            SpawnPlayerPacket* spawn_player_packet = static_cast<SpawnPlayerPacket*>(packet.get());
            std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), L"PlayerCharacter", spawn_player_packet->character_info.unique_id);
            if (IsValid(player_character))
            {
                float position_x = spawn_player_packet->position_x;
                float position_y = spawn_player_packet->position_y;

                player_character->InitSpawn({position_x, position_y});
            }
        }
        break;

    case DestroyPlayerPacket::StaticPacketID:
        {
            DestroyPlayerPacket* destroy_player_packet = static_cast<DestroyPlayerPacket*>(packet.get());
            DestroyNetworkActor(destroy_player_packet->unique_id);
        }
        break;

    case MovePlayerPacket::StaticPacketID:
        {
            MovePlayerPacket* move_player_packet = static_cast<MovePlayerPacket*>(packet.get());
            
            std::shared_ptr<NetworkActor> network_actor = GetNetworkActor(move_player_packet->unique_id);
            if (IsValid(network_actor)) network_actor->ReceivePacket(packet.get());
        }
        break;
        
    default:
        break;
    }
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
