#include "pch.h"
#include "GameMap.h"

#include <CustomPacket.h>

#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actors/TilemapLoader.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Asset/AssetManager.h"
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Level/CameraManager.h"
#include "Subsystems/GameSubsystem.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Subsystems/PlayerSubsystem.h"
#include "Subsystems/SessionSubsystem.h"
#include "UI/UI.h"
#include "UI/UIInGameState.h"
#include "UI/Element/UIMiniMap.h"

GameMap::GameMap(const std::wstring& kName) :
    Level(kName)
{
}

void GameMap::Load()
{
    Level::Load();

    UI::Get()->ChangeState(UIInGameState::StaticClass());

    PlayerSubsystem* player_subsystem = PlayerSubsystem::Get();
    CameraManager* camera_manager = CameraManager::Get();

#pragma region 타일맵
    std::shared_ptr<TilemapLoader> tilemap_loader = World::Get()->SpawnActor<TilemapLoader>(TilemapLoader::StaticClass());
    if (IsValid(tilemap_loader))
    {
        std::wstring wide_str = std::format(L"{:06}", player_subsystem->map_id_);
        Tilemap* tilemap = AssetManager::Get()->Load<Tilemap>(L"Tilemaps\\" + wide_str + L".tmx");
        if (tilemap)
        {
            tilemap_loader->SetTilemap(tilemap);

            Bounds bounds = tilemap->GetWorldBounds();
            camera_manager->SetLimit(bounds.size.x, bounds.size.y);

            if (auto* state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
                state->GetMiniMap()->SetTilemap(tilemap);

            GameSubsystem::Get()->PlayBGM(tilemap->GetBGM());
        }
    }
#pragma endregion

#pragma region 플레이어 캐릭터
    std::shared_ptr<PlayerCharacter> player_character = AddActor<PlayerCharacter>(PlayerCharacter::StaticClass(), L"PlayerCharacter");
    if (IsValid(player_character))
    {
        player_character->SetObjectID(player_subsystem->GetCharacterID());
        player_character->Init(player_subsystem->name_, player_subsystem->body_color_, player_subsystem->spawn_position);

        camera_manager->SetTarget(player_character);
        NetworkSubsystem::Get()->SetPlayerCharacter(player_character);
    }
#pragma endregion
    
    MapLoadCompletePacket packet;
    SessionSubsystem::Get()->SendPacket(packet);
}

void GameMap::Unload(EndPlayReason type)
{
    Level::Unload(type);
    
    AudioManager* audio_manager = AudioManager::Get();
    // audio_manager->StopSound(ChannelGroup::kSE);
    audio_manager->StopSound(ChannelGroup::kMobSE);
    audio_manager->StopSound(ChannelGroup::kSkillSE);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameMap>("GameMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
