#include "pch.h"
#include "MainMenu.h"

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
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
