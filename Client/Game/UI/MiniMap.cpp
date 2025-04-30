#include "pch.h"
#include "MiniMap.h"

#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Tilemap/TilemapComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Level/CameraManager.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "Subsystems/NetworkSubsystem.h"
#include "Windows/DX/Renderer.h"

UI::MiniMap::MiniMap(const std::wstring& name) :
    Widget(name)
{
}

std::shared_ptr<UI::MiniMap> UI::MiniMap::Create(const std::wstring& name)
{
    return std::make_shared<MiniMap>(name);
}

void UI::MiniMap::Render(Renderer* renderer, WindowsWindow* window)
{
    Widget::Render(renderer, window);

    Math::Rect rect = GetRect();

    renderer->DrawSolidBox(window, rect, GetPivotPosition(), {0, 0, 0, 100});

    Bounds map_bounds = { {0.f, 0.f}, {25.f, 19.f} };
    Math::Vector2 scale = {rect.width / (map_bounds.max.x - map_bounds.min.x), rect.height / (map_bounds.max.y - map_bounds.min.y)};

    Math::Vector2 position = GET_NETWORK()->GetLocalPlayer()->GetTransform()->GetPosition();
    float x = (position.x - map_bounds.min.x) * scale.x;
    float y = (map_bounds.max.y - position.y) * scale.y;

    Math::Rect marker_rect = GetRect(
        {x, y},
        {5.f, 5.f},
        {.5f, .5f}
    );

    Math::Vector2 pivot_position = GetPivotPosition(marker_rect, {.5f, .5f});
    
    renderer->BeginLayer(rect);
    // renderer->DrawSolidBox(window, marker_rect, pivot_position, Math::Color::Yellow);
    for (const auto& network_actor : GET_NETWORK()->GetNetworkActors())
    {
        std::shared_ptr<NetworkActor> actor = network_actor.second;
        Math::Vector2 p = actor->GetTransform()->GetPosition();
        float _x = (p.x - map_bounds.min.x) * scale.x;
        float _y = (map_bounds.max.y - p.y) * scale.y;
        Math::Rect actor_rect = GetRect(
            {_x, _y},
            {5.f, 5.f},
            {.5f, .5f}
        );
        Math::Vector2 actor_pivot_position = GetPivotPosition(actor_rect, {.5f, .5f});
        renderer->DrawSolidBox(window, actor_rect, actor_pivot_position, Math::Color::White);
    }
    
    const std::vector<Bounds>& collision_bodies = GET_NETWORK()->GetTilemapComponent()->GetCollisionBodies();
    for (const auto& body : collision_bodies)
    {
        float _x = (body.center.x - map_bounds.min.x) * scale.x;
        float _y = (map_bounds.max.y - body.center.y) * scale.y;
        Math::Rect body_rect = GetRect(
            {_x, _y},
            {body.size.x * scale.x, body.size.y * scale.y},
            {.5f, .5f}
        );

        Math::Vector2 body_pivot_position = GetPivotPosition(body_rect, {.5f, .5f});
        renderer->DrawSolidBox(window, body_rect, body_pivot_position, Math::Color::Red);
    }
    renderer->EndLayer();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::MiniMap>("UI::MiniMap")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
