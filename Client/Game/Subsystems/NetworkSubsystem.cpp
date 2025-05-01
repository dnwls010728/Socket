#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>
#include <ranges>

#include "GameInstance.h"
#include "SessionSubsystem.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actors/TilemapLoader.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Asset/AssetManager.h"
#include "Input/Keyboard.h"
#include "Level/CameraManager.h"
#include "UI/Widget/ListBox.h"

NetworkSubsystem::NetworkSubsystem() :
    network_actors_(),
    local_player_(),
    tilemap_(nullptr),
    tilemap_component_()
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

    SessionSubsystem* session_subsystem = GET_SESSION();
    session_subsystem->packet_handler.Add(this, &NetworkSubsystem::ProcessPackets);

    if (session_subsystem->IsInGame())
    {
        InGameReadyPacket packet;
        SendPacket(packet);
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
            if (response->is_success) TransitionMap(response->map_unique_id);
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

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet.get());
            for (const auto& network_actor : network_actors_)
            {
                if (network_actor.second->GetUniqueID() == chat_message_packet->unique_id)
                {
                    std::shared_ptr<PlayerCharacter> player_character = std::dynamic_pointer_cast<PlayerCharacter>(network_actor.second);
                    if (IsValid(player_character))
                    {
                        std::wstring message = chat_message_packet->message;
                        player_character->Speak(message);
                    }
                    break;
                }
            }
        }
        break;
        
    default:
        break;
    }
}

void NetworkSubsystem::TransitionMap(uint32_t map_unique_id)
{
    std::vector<Actor*> actors = {};
    World::Get()->GetActors(Actor::StaticClass(), actors);

    for (const auto& actor : actors)
    {
        if (IsValid(actor)) actor->Destroy();
    }

    network_actors_.clear();
    local_player_.reset();

    CameraManager* camera_manager = CameraManager::Get();

    std::shared_ptr<TilemapLoader> tilemap_loader = World::Get()->SpawnActor<TilemapLoader>(TilemapLoader::StaticClass());
    if (IsValid(tilemap_loader))
    {
        std::wstring wide_str = std::format(L"{:06}", map_unique_id);
        tilemap_ = AssetManager::Get()->Load<Tilemap>(L"Tilemaps\\" + wide_str + L".tmx");
        if (tilemap_)
        {
            tilemap_loader->SetTilemap(tilemap_);

            camera_manager->SetSize(6.f);
            camera_manager->SetTickType(TickType::kTick);

            Math::Vector2 map_size = tilemap_->GetMapSize();
            camera_manager->SetLimit(map_size.x, map_size.y);
        }
    }
    
    const CharacterInfo& character_info = GET_SESSION()->GetCharacterInfo();
    std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), L"PlayerCharacter", character_info.unique_id);
    if (IsValid(player_character))
    {
        player_character->SetMine(true);
        local_player_ = player_character;
        
        camera_manager->SetTarget(player_character);
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
