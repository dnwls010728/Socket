#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>
#include <ranges>

#include "DebugDrawHelper.h"
#include "GameInstance.h"
#include "InGameUISubsystem.h"
#include "PlayerSubsystem.h"
#include "SessionSubsystem.h"
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

void NetworkSubsystem::OnWorldBeginPlay()
{
    WorldSubsystem::OnWorldBeginPlay();

    SessionSubsystem* session_subsystem = SessionSubsystem::Get();
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

void NetworkSubsystem::TransitionMap(uint32_t map_id)
{
    ObjectPoolSubsystem::Get()->ClearPool();
    
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
        player_character->Init(player_subsystem->GetName(), player_subsystem->GetInitialPosition());
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
