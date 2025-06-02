#include "pch.h"
#include "MiniMap.h"

#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/Tilemap.h"
#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Level/CameraManager.h"
#include "Math/Color.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Windows/DX/Renderer.h"
#include "Windows/DX/UITexture.h"

UI_OLD::MiniMap::MiniMap(const std::wstring& name) :
    Widget(name),
    tilemap_(nullptr)
{
}

void UI_OLD::MiniMap::SetTilemap(Tilemap* tilemap)
{
    tilemap_ = tilemap;
    if (!tilemap_) return;

    Math::Vector2 map_size = tilemap_->GetMapSize();
    Math::Vector2 tile_size = tilemap_->GetTileSize();

    // 4배 축소
    float width = (map_size.x * tile_size.x) / 4.f;
    float height = (map_size.y * tile_size.y) / 4.f;

    SetSize({ width, height });
}

std::shared_ptr<UI_OLD::MiniMap> UI_OLD::MiniMap::Create(const std::wstring& name)
{
    return std::make_shared<MiniMap>(name);
}

void UI_OLD::MiniMap::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    Math::Rect rect = GetRect();
    renderer->DrawSolidBox(window, rect, GetPivotPosition(), {0, 0, 0, 100});

    if (tilemap_)
    {
        renderer->DrawBitmap(window, tilemap_->GetUITexture()->GetTexture(), rect, GetPivotPosition());

        Bounds map_world_bounds = tilemap_->GetWorldBounds();
        Math::Vector2 scale { rect.width / (map_world_bounds.max.x - map_world_bounds.min.x), rect.height / (map_world_bounds.max.y - map_world_bounds.min.y) };

        NetworkSubsystem* network_subsystem = NetworkSubsystem::Get();

        renderer->BeginLayer(rect);

        // 플레이어 마커
        {
            Math::Vector2 position = network_subsystem->GetPlayer()->GetTransform()->GetPosition();
            position.x = (position.x - map_world_bounds.min.x) * scale.x;
            position.y = (map_world_bounds.max.y - position.y) * scale.y;

            renderer->DrawSolidCircle(window, position, 5.f, Math::Color::Yellow);
            renderer->DrawCircle(window, position, 5.f, Math::Color::White);
        }

        // 다른 플레이어 마커
        {
            std::vector<std::shared_ptr<PlayerCharacter>> players = {};
            network_subsystem->GetOtherPlayers(players);

            for (const auto& player : players)
            {
                Math::Vector2 position = player->GetTransform()->GetPosition();
                position.x = (position.x - map_world_bounds.min.x) * scale.x;
                position.y = (map_world_bounds.max.y - position.y) * scale.y;

                renderer->DrawSolidCircle(window, position, 5.f, Math::Color::Red);
                renderer->DrawCircle(window, position, 5.f, Math::Color::White);
            }
        }

        renderer->DrawString(window, tilemap_->GetName(), rect, GetPivotPosition(), Math::Color::White, 0.f, L"NanumBarunGothic", 12.f);
        
        renderer->EndLayer();
    }

    renderer->DrawBox(window, rect, GetPivotPosition(), Math::Color::White);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI_OLD::MiniMap>("UI::MiniMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
