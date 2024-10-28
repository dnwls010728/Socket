#include "pch.h"
#include "MainMenu.h"

#include "Logger.h"
#include "Level/World.h"
#include "UI/Canvas.h"
#include "Windows/WindowsWindow.h"
#include "../SocketCore/SocketSession.h"
#include "UI/Widget/Text.h"
#include "Windows/DX/UITexture.h"
MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    Text* text = Canvas::Get()->AddWidget<Text>(L"Text");
    text->SetAlignment(Text::kMiddleCenter);
    text->SetText(L"Hello, World!");
    text->on_drag_start.Add([](const Math::Vector2& start_position)
    {
        Logger::Print(L"Start position: %f, %f", start_position.x, start_position.y);
    });

    text->on_drag.Add([](const Math::Vector2& delta)
    {
        Logger::Print(L"Delta: %f, %f", delta.x, delta.y);
    });

    text->on_drag_end.Add([](const Math::Vector2& end_position)
    {
        Logger::Print(L"End position: %f, %f", end_position.x, end_position.y);
    });
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
