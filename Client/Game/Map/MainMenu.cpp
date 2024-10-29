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
    Widget* root_widget = Canvas::Get()->AddWidget<Widget>(L"Root");
    root_widget->SetAnchorPreset(AnchorPreset::kStretch);
    root_widget->SetSize({0.f, 0.f});
    
    text_ = Canvas::Get()->AddWidget<Text>(L"Text");
    text_->SetAlignment(Text::kMiddleCenter);
    text_->SetText(L"Hello, World!");
    text_->AttachToWidget(root_widget);

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
