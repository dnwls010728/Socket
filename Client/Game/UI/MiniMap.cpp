#include "pch.h"
#include "MiniMap.h"

#include "Actor/Component/TransformComponent.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Level/CameraManager.h"
#include "Math/Color.h"
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

    EngineSettings* settings = EngineSettings::Get();
    
    float width = settings->GetScreenWidth();
    float height = settings->GetScreenHeight();
    float width_ratio = rect.width / width;
    float height_ratio = rect.height / height;

    renderer->DrawSolidBox(window, rect, GetPivotPosition(), Math::Color::White);
    
    renderer->BeginLayer(rect);
    for (const auto& network_actor : GET_NETWORK()->GetNetworkActors())
    {
        std::shared_ptr<NetworkActor> actor = network_actor.second;
        if (IsValid(actor))
        {
            Math::Vector2 local_player_position = actor->GetTransform()->GetPosition();
            Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(local_player_position);

            float x = screen_position.x * width_ratio;
            float y = screen_position.y * height_ratio;
            
            renderer->DrawBox(window, Math::Rect(x + rect.x, y + rect.y, 5.f, 5.f), GetPivotPosition(), Math::Color::Red);
        }
    }
    renderer->EndLayer();
    
    renderer->DrawBox(window, rect, GetPivotPosition(), Math::Color::Black);
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
