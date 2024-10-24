#include "pch.h"
#include "MainMenu.h"

#include "Level/World.h"
#include "UI/Canvas.h"
#include "Windows/WindowsWindow.h"
#include "../SocketCore/SocketSession.h"
#include "Windows/DX/UITexture.h"
MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
