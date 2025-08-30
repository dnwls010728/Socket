#include "pch.h"
#include "GameMap.h"

#include <CustomPacket.h>

#include "DebugDrawHelper.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actors/TilemapLoader.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Asset/AssetManager.h"
#include "Audio/AudioManager.h"
#include "Level/CameraManager.h"
#include "Math/Math.h"
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
    tilemap_loader_ = World::Get()->SpawnActor<TilemapLoader>(TilemapLoader::StaticClass());
    if (IsValid(tilemap_loader_))
    {
        std::wstring wide_str = std::format(L"{:06}", player_subsystem->map_id_);
        Tilemap* tilemap = AssetManager::Get()->Load<Tilemap>(L"Tilemaps\\" + wide_str + L".tmx");
        if (tilemap)
        {
            tilemap_loader_->SetTilemap(tilemap);

            Bounds bounds = tilemap->GetWorldBounds();
            camera_manager->SetLimit(bounds.size.x, bounds.size.y);

            if (auto* state = UI::Get()->GetState())
            {
                if (auto* element = state->FindElement<UIMiniMap>(L"MiniMap"))
                    element->SetTilemap(tilemap);
            }

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

void GameMap::Tick(float deltaTime)
{
    Level::Tick(deltaTime);

    Mouse* mouse = Mouse::Get();
    Math::Vector2 mouse_position = mouse->GetMousePosition();
    Math::Vector2 world_position = Renderer::Get()->ScreenToWorld(mouse_position);

    if (IsValid(tilemap_loader_))
    {
        const auto& tilemap_component = tilemap_loader_->GetTilemapComponent();
        if (tilemap_component)
        {
            Math::Vector2i cell = tilemap_component->WorldToCell(world_position);
            Math::Vector2 cell_center = tilemap_component->GetCellCenter(cell);
            DebugDrawHelper::Get()->DrawBox(cell_center, Math::Vector2::One(), Math::Color::Red);
        }
    }
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
