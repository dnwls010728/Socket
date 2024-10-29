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
    
    Canvas::Get()->SetRootWidget(root_widget);
    
    text_ = Canvas::Get()->AddWidget<Text>(L"Text");
    text_->SetAlignment(Text::kMiddleCenter);
    text_->SetText(L"Hello, World!");
    text_->AttachToWidget(root_widget);
    text_->SetRayCastTarget(true);
    text_->OnMousePressed.Add([]()
    {
        Logger::Print(L"Text pressed!");
    });

    text_->OnMouseReleased.Add([]()
    {
        Logger::Print(L"Text released!");
    });
    
    text_->OnDragStart.Add([](const Math::Vector2& start_position)
    {
        Logger::Print(L"Start position: %f, %f", start_position.x, start_position.y);
    });

    text_->OnDrag.Add([=](const Math::Vector2& delta)
    {
        Math::Vector2 new_position = text_->GetAnchoredPosition() + delta;
        text_->SetAnchoredPosition(new_position);

        Logger::Print(L"Delta: %f, %f", delta.x, delta.y);
    });

    text_->OnDragEnd.Add([](const Math::Vector2& end_position)
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
