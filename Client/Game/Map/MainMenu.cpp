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
#include "Resource/ResourceManager.h"
#include "UI/Widget/Image.h"
#include "Windows/DX/UITexture.h"

MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    Canvas* canvas = Canvas::Get();

    UI::Text* build = canvas->AddWidget<UI::Text>(L"Build");
    build->SetAnchorPreset(UI::AnchorPresets::kLeft | UI::AnchorPresets::kBottom, true);
    build->SetAnchoredPosition({10.f, 0.f});
    build->SetSize({200.f, 30.f});
    build->SetText(L"Development Build: v0.0.1");
    build->SetAlignment(UI::TextAnchor::kMiddleLeft);
    
    UI::TextBox* tb_id = canvas->AddWidget<UI::TextBox>(L"ID");
    tb_id->SetAnchoredPosition({0.f, -45.f});
    tb_id->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    tb_id->SetSize({200.f, 30.f});
    tb_id->SetPlaceholder(L"ID");

    UI::TextBox* tb_pw = canvas->AddWidget<UI::TextBox>(L"Password");
    tb_pw->SetAnchoredPosition({0.f, 0.f});
    tb_pw->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    tb_pw->SetSize({200.f, 30.f});
    tb_pw->SetPlaceholder(L"Password");
    tb_pw->SetContentType(UI::ContentType::kPassword);

    UI::Button* start_button = canvas->AddWidget<UI::Button>(L"Login");
    start_button->SetAnchoredPosition({0.f, 45.f});
    start_button->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    start_button->on_click.Add([]()
    {
        GSocketSession->Connect();
        World::Get()->OpenLevel(LevelType::kDefault);
    });

    UI::Text* button_text = canvas->AddWidget<UI::Text>(L"Login Text");
    button_text->SetAnchoredPosition({0.f, 0.f});
    button_text->SetSize({0.f, 0.f});
    button_text->SetAnchorPreset(UI::AnchorPresets::kStretch, true);
    button_text->AttachToWidget(start_button);
    button_text->SetText(L"Login");
    button_text->SetAlignment(UI::TextAnchor::kMiddleCenter);
    button_text->SetColor(Math::Color::Black);

    UI::Button* exit_button = canvas->AddWidget<UI::Button>(L"Exit");
    exit_button->SetAnchoredPosition({0.f, 90.f});
    exit_button->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    exit_button->on_click.Add([]()
    {
        WindowsWindow* window = World::Get()->GetWindow();
        PostMessage(window->GetHWnd(), WM_USER, 0, 0);
    });

    UI::Text* exit_button_text = canvas->AddWidget<UI::Text>(L"Exit Text");
    exit_button_text->SetAnchoredPosition({0.f, 0.f});
    exit_button_text->SetSize({0.f, 0.f});
    exit_button_text->SetAnchorPreset(UI::AnchorPresets::kStretch, true);
    exit_button_text->AttachToWidget(exit_button);
    exit_button_text->SetText(L"Exit");
    exit_button_text->SetAlignment(UI::TextAnchor::kMiddleCenter);
    exit_button_text->SetColor(Math::Color::Black);

    UITexture* texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\9SliceSprites-4.jpg");
    
    UI::Image* image = canvas->AddWidget<UI::Image>(L"Image");
    image->SetAnchorPreset(UI::AnchorPresets::kCenter | UI::AnchorPresets::kMiddle, true);
    image->SetSize({200.f, 200.f});
    image->SetTexture(texture);
    image->SetDrawMode(UI::DrawMode::Sliced);
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
