#include "pch.h"
#include "MainMenu.h"

#include "Level/World.h"
#include "UI/Canvas.h"
#include "UI/Widget.h"
#include "Windows/WindowsWindow.h"

MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    Widget* root_widget = Canvas::Get()->AddWidget<Widget>(L"Root");
    root_widget->SetAnchorPreset(AnchorPreset::kStretch);
    root_widget->SetSize({0.f, 0.f});
    
    Canvas::Get()->SetRootWidget(root_widget);
    
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
