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

MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    Canvas* canvas = Canvas::Get();

    UI::Text* build = canvas->AddWidget<UI::Text>(L"Build");
    build->SetAnchorPreset(UI::AnchorPresets::kLeft | UI::AnchorPresets::kBottom, true);
    build->SetPosition({10.f, 0.f});
    build->SetSize({200.f, 30.f});
    build->SetText(L"Development Build: v0.0.1");
    build->SetAlignment(UI::TextAnchor::kMiddleLeft);
    
    std::shared_ptr<UI::TextBox> tb_id = std::make_shared<UI::TextBox>(L"ID");
    tb_id->SetPosition({0.f, -45.f});
    tb_id->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    tb_id->SetSize({200.f, 30.f});
    tb_id->SetPlaceholder(L"ID");

    Canvas::Get()->AddWidget(tb_id);

    std::shared_ptr<UI::TextBox> tb_pw = std::make_shared<UI::TextBox>(L"Password");
    tb_pw->SetPosition({0.f, 0.f});
    tb_pw->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    tb_pw->SetSize({200.f, 30.f});
    tb_pw->SetPlaceholder(L"Password");
    tb_pw->SetContentType(UI::ContentType::kPassword);

    Canvas::Get()->AddWidget(tb_pw);

    UI::Button* start_button = canvas->AddWidget<UI::Button>(L"Login");
    start_button->SetPosition({0.f, 45.f});
    start_button->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    start_button->on_click.Add([]()
    {
        World::Get()->OpenLevel(LevelType::kDefault);
    });

    UI::Text* button_text = canvas->AddWidget<UI::Text>(L"Login Text");
    button_text->SetPosition({0.f, 0.f});
    button_text->SetSize({0.f, 0.f});
    button_text->SetAnchorPreset(UI::AnchorPresets::kStretch, true);
    button_text->AttachToWidget(start_button);
    button_text->SetText(L"Login");
    button_text->SetAlignment(UI::TextAnchor::kMiddleCenter);
    button_text->SetColor(Math::Color::Black);

    std::shared_ptr<UI::Button> exit_button = std::make_shared<UI::Button>(L"Exit");
    exit_button->SetPosition({0.f, 90.f});
    exit_button->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    exit_button->on_click.Add([]()
    {
        WindowsWindow* window = World::Get()->GetWindow();
        PostMessage(window->GetHWnd(), WM_USER, 0, 0);
    });

    Canvas::Get()->AddWidget(exit_button);

    std::shared_ptr<UI::Text> exit_button_text = std::make_shared<UI::Text>(L"Exit Text");
    exit_button_text->SetPosition({0.f, 0.f});
    exit_button_text->SetSize({0.f, 0.f});
    exit_button_text->SetAnchorPreset(UI::AnchorPresets::kStretch, true);
    exit_button_text->AttachToWidget(exit_button.get());
    exit_button_text->SetText(L"Exit");
    exit_button_text->SetAlignment(UI::TextAnchor::kMiddleCenter);
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
