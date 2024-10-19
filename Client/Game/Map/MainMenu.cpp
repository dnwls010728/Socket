#include "pch.h"
#include "MainMenu.h"

#include "Core.h"
#include "Enums.h"
#include "Level/World.h"
#include "UI/Canvas.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/Text.h"
#include "UI/Widget/TextBox.h"
#include "Windows/WindowsWindow.h"
#include "..\SocketSession.h"
MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    std::shared_ptr<UI::Text> build = std::make_shared<UI::Text>(L"Build");
    build->SetAnchorPreset(AnchorPresets::kLeft | AnchorPresets::kBottom, true);
    build->SetSize({200.f, 30.f});
    build->SetText(L"Development Build: v0.0.1");

    Canvas::Get()->AddWidget(build);
    
    std::shared_ptr<UI::TextBox> tb_id = std::make_shared<UI::TextBox>(L"ID");
    tb_id->SetPosition({0.f, -45.f});
    tb_id->SetAnchorPreset(AnchorPresets::kMiddle | AnchorPresets::kCenter, true);
    tb_id->SetSize({200.f, 30.f});
    tb_id->SetPlaceholder(L"ID");

    Canvas::Get()->AddWidget(tb_id);

    std::shared_ptr<UI::TextBox> tb_pw = std::make_shared<UI::TextBox>(L"Password");
    tb_pw->SetPosition({0.f, 0.f});
    tb_pw->SetAnchorPreset(AnchorPresets::kMiddle | AnchorPresets::kCenter, true);
    tb_pw->SetSize({200.f, 30.f});
    tb_pw->SetPlaceholder(L"Password");
    tb_pw->SetContentType(UI::ContentType::kPassword);

    Canvas::Get()->AddWidget(tb_pw);
    
    std::shared_ptr<UI::Button> start_button = std::make_shared<UI::Button>(L"Login");
    start_button->SetPosition({0.f, 45.f});
    start_button->SetAnchorPreset(AnchorPresets::kMiddle | AnchorPresets::kCenter, true);
    start_button->on_click.Add([]()
    {
            GSocketSession->Connect();
        World::Get()->OpenLevel(LevelType::kDefault);
    });

    Canvas::Get()->AddWidget(start_button);

    std::shared_ptr<UI::Text> button_text = std::make_shared<UI::Text>(L"Login Text");
    button_text->SetPosition({0.f, 0.f});
    button_text->SetSize({0.f, 0.f});
    button_text->SetAnchorPreset(AnchorPresets::kStretch, true);
    button_text->AttachToWidget(start_button.get());
    button_text->SetText(L"Login");
    button_text->SetColor(Math::Color::Black);

    Canvas::Get()->AddWidget(button_text);

    std::shared_ptr<UI::Button> exit_button = std::make_shared<UI::Button>(L"Exit");
    exit_button->SetPosition({0.f, 90.f});
    exit_button->SetAnchorPreset(AnchorPresets::kMiddle | AnchorPresets::kCenter, true);
    exit_button->on_click.Add([]()
    {
        WindowsWindow* window = World::Get()->GetWindow();
        PostMessage(window->GetHWnd(), WM_USER, 0, 0);
    });

    Canvas::Get()->AddWidget(exit_button);

    std::shared_ptr<UI::Text> exit_button_text = std::make_shared<UI::Text>(L"Exit Text");
    exit_button_text->SetPosition({0.f, 0.f});
    exit_button_text->SetSize({0.f, 0.f});
    exit_button_text->SetAnchorPreset(AnchorPresets::kStretch, true);
    exit_button_text->AttachToWidget(exit_button.get());
    exit_button_text->SetText(L"Exit");
    exit_button_text->SetColor(Math::Color::Black);

    Canvas::Get()->AddWidget(exit_button_text);
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
