#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>
#include <ranges>

#include "GameInstance.h"
#include "InGameUISubsystem.h"
#include "SessionSubsystem.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actors/TilemapLoader.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Asset/AssetManager.h"
#include "imgui/imgui.h"
#include "Input/Keyboard.h"
#include "Level/CameraManager.h"
#include "UI/MiniMap.h"
#include "UI/Widget/ListBox.h"

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

        InGameUISubsystem* in_game_ui_subsystem = GET_IN_GAME_UI();
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
    GET_SESSION()->SendPacket(packet);
}

void NetworkSubsystem::ChangeMap(uint32_t map_unique_id)
{
    ChangeMapRequest request;
    request.map_unique_id = map_unique_id;
    SendPacket(request);
}

std::shared_ptr<NetworkActor> NetworkSubsystem::SpawnNetworkActor(const std::wstring& type_name, uint32_t unique_id, const std::wstring& name)
{
    std::string type_name_a(type_name.begin(), type_name.end());
    rttr::type type = rttr::type::get_by_name(type_name_a);
    if (!type.is_valid())
    {
        return nullptr;
    }
    return SpawnNetworkActor<NetworkActor>(type, unique_id, name);
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

std::shared_ptr<NetworkActor> NetworkSubsystem::GetNetworkActor(const uint32_t unique_id)
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
            std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), spawn_player_packet->character_info.unique_id);
            if (IsValid(player_character))
            {
                const CharacterInfo character_info = spawn_player_packet->character_info;
                
                float position_x = spawn_player_packet->position_x;
                float position_y = spawn_player_packet->position_y;

                player_character->InitSpawn(character_info.name, {position_x, position_y});
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
            
            std::shared_ptr<NetworkActor> network_actor = GetNetworkActor(move_player_packet->unique_id);
            if (IsValid(network_actor)) network_actor->ReceivePacket(packet.get());
        }
        break;

    case ChatMessagePacket::StaticPacketID:
        {
            ChatMessagePacket* chat_message_packet = static_cast<ChatMessagePacket*>(packet.get());
            for (const auto& network_actor : network_actors_ | std::views::values)
            {
                if (network_actor->GetUniqueID() == chat_message_packet->unique_id)
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
            ObjectInfo  &object_info = spawn_object_packet->object;
            std::shared_ptr<NetworkActor> new_object = SpawnNetworkActor(object_info.type_name, object_info.unique_id, object_info.name);
            if (IsValid(new_object))
            {
                new_object->GetTransform()->SetPosition({object_info.last_position_x,object_info.last_position_y});
            }
        }
        break;
    case ObjectPositionPacket::StaticPacketID:
        {
            ObjectPositionPacket* object_position_packet = static_cast<ObjectPositionPacket*>(packet.get());
            
            std::shared_ptr<NetworkActor> network_actor = GetNetworkActor(object_position_packet->unique_id);
            if (IsValid(network_actor)) network_actor->ReceivePacket(packet.get());
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

    other_players_.clear();
    player_.reset();
    network_actors_.clear();

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
            camera_manager->SetTickType(TickType::kPhysicsTick);

            Bounds bounds = tilemap_->GetWorldBounds();
            camera_manager->SetLimit(bounds.size.x, bounds.size.y);
        }
    }
    
    const CharacterInfo& character_info = GET_SESSION()->GetCharacterInfo();
    std::shared_ptr<PlayerCharacter> player_character = SpawnNetworkActor<PlayerCharacter>(PlayerCharacter::StaticClass(), character_info.unique_id);
    if (IsValid(player_character))
    {
        player_character->InitSpawn(character_info.name, Math::Vector2::Zero());
        player_character->SetMine(true);
        
        player_ = player_character;
        
        camera_manager->SetTarget(player_character);
    }

    InGameUISubsystem* in_game_ui_subsystem = GET_IN_GAME_UI();
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
