#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>
#include <ranges>

#include "DebugDrawHelper.h"
#include "GameInstance.h"
#include "InGameUISubsystem.h"
#include "PlayerSubsystem.h"
#include "SessionSubsystem.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actors/TilemapLoader.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Mobs/MobBase.h"
#include "Asset/AssetManager.h"
#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "Level/CameraManager.h"
#include "ObjectPool/ObjectPoolSubsystem.h"
#include "UI/MiniMap.h"

NetworkSubsystem::NetworkSubsystem() :
    network_actors_(),
    player_(),
    other_players_(),
    tilemap_(nullptr)
{
}

void NetworkSubsystem::Init()
{
    WorldSubsystem::Init();
    
}

void NetworkSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    SessionSubsystem::Get()->packet_handler.Remove(this, &NetworkSubsystem::ProcessPackets);
}

void NetworkSubsystem::OnWorldBeginPlay()
{
    WorldSubsystem::OnWorldBeginPlay();

    SessionSubsystem* session_subsystem = SessionSubsystem::Get();
    session_subsystem->packet_handler.Add(this, &NetworkSubsystem::ProcessPackets);

    if (session_subsystem->IsInGame())
    {
        InGameUISubsystem* in_game_ui_subsystem = InGameUISubsystem::Get();
        in_game_ui_subsystem->ShowMiniMap();
        in_game_ui_subsystem->ShowChatUI();
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
    SessionSubsystem::Get()->SendPacket(packet);
}

void NetworkSubsystem::ChangeMap(uint32_t map_id)
{
    ChangeMapRequest request;
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

void NetworkSubsystem::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
{
    switch (packet->GetPacketID())
    {
    case ChangeMapResponse::StaticPacketID:
        {
            ChangeMapResponse* response = static_cast<ChangeMapResponse*>(packet.get());
            if (response->is_success) TransitionMap(response->map_id);
        }
        break;
        
    case SpawnPlayerPacket::StaticPacketID:
        {
            SpawnPlayerPacket* spawn_player_packet = static_cast<SpawnPlayerPacket*>(packet.get());
            std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), spawn_player_packet->character_id);
            if (IsValid(player_character))
            {
                float position_x = spawn_player_packet->position_x;
                float position_y = spawn_player_packet->position_y;

                player_character->InitSpawn(spawn_player_packet->name, {position_x, position_y});
                other_players_.emplace_back(player_character);
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
            
            std::shared_ptr<NetworkActor> network_actor = FindNetworkActor(move_player_packet->unique_id);
            if (IsValid(network_actor)) network_actor->ReceivePacket(packet.get());
        }
        break;

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet.get());
            for (const auto& network_actor : network_actors_ | std::views::values)
            {
                if (network_actor->GetObjectID() == chat_message_packet->unique_id)
                {
                    std::shared_ptr<PlayerCharacter> player_character = std::dynamic_pointer_cast<PlayerCharacter>(network_actor);
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

    case SpawnObjectPacket::StaticPacketID:
        {
            SpawnObjectPacket* spawn_object_packet = static_cast<SpawnObjectPacket*>(packet.get());

            ObjectInfo& object_info = spawn_object_packet->object_info;

            std::shared_ptr<Actor> out_actor = nullptr;
            if (ObjectPoolSubsystem::Get()->GetFromPool(MobBase::StaticClass(), out_actor))
            {
                if (IsValid(out_actor))
                {
                    std::shared_ptr<NetworkActor> network_actor = std::dynamic_pointer_cast<NetworkActor>(out_actor);
                    if (IsValid(network_actor))
                    {
                        network_actor->SetObjectID(object_info.object_id);
                        network_actor->GetTransform()->SetPosition({object_info.position_x, object_info.position_y});
                    }
                }
            }
        }
        break;

    case DestroyObjectPacket::StaticPacketID:
        {
            DestroyObjectPacket* destroy_object_packet = static_cast<DestroyObjectPacket*>(packet.get());
            
            std::shared_ptr<NetworkActor> network_actor = FindNetworkActor(destroy_object_packet->object_id);
            if (IsValid(network_actor)) ObjectPoolSubsystem::Get()->ReturnToPool(network_actor);
        }
        break;
        
    case ObjectPositionPacket::StaticPacketID:
        {
            ObjectPositionPacket* object_position_packet = static_cast<ObjectPositionPacket*>(packet.get());
            
            std::shared_ptr<NetworkActor> network_actor = FindNetworkActor(object_position_packet->object_id);
            if (IsValid(network_actor)) network_actor->ReceivePacket(packet.get());
        }
        break;
        
    default:
        break;
    }
}

void NetworkSubsystem::TransitionMap(uint32_t map_id)
{
    std::vector<Actor*> actors = {};
    World::Get()->GetActors(Actor::StaticClass(), actors);

    for (const auto& actor : actors)
    {
        if (IsValid(actor)) actor->Destroy();
    }

    other_players_.clear();
    player_.reset();
    network_actors_.clear();

    CameraManager* camera_manager = CameraManager::Get();

    std::shared_ptr<TilemapLoader> tilemap_loader = World::Get()->SpawnActor<TilemapLoader>(TilemapLoader::StaticClass());
    if (IsValid(tilemap_loader))
    {
        std::wstring wide_str = std::format(L"{:06}", map_id);
        tilemap_ = AssetManager::Get()->Load<Tilemap>(L"Tilemaps\\" + wide_str + L".tmx");
        if (tilemap_)
        {
            tilemap_loader->SetTilemap(tilemap_);

            camera_manager->SetSize(6.f);
            camera_manager->SetTickType(TickType::kPhysicsTick);

            Bounds bounds = tilemap_->GetWorldBounds();
            camera_manager->SetLimit(bounds.size.x, bounds.size.y);
        }
    }
    
    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();
    std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), player_subsystem->GetCharacterID());
    if (IsValid(player_character))
    {
        player_character->InitSpawn(player_subsystem->GetName(), player_subsystem->GetInitialPosition());
        player_character->SetMine(true);
        
        player_ = player_character;
        
        camera_manager->SetTarget(player_character);
    }

    InGameUISubsystem* in_game_ui_subsystem = InGameUISubsystem::Get();
    in_game_ui_subsystem->GetMiniMap()->SetTilemap(tilemap_);
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
