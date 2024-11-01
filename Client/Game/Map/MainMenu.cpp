#include "pch.h"
#include "MainMenu.h"

#include "Level/World.h"
#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget.h"
#include "UI/Widget/Button.h"
#include "UI/Widget/Text.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/UITexture.h"

MainMenu::MainMenu(const std::wstring& kName) :
    Level(kName)
{
}

void MainMenu::Load()
{
    Level::Load();
    
    Canvas* canvas = Canvas::Get();

    UITexture* texture = ResourceManager::Get()->Load<UITexture>(L"Sprites\\UI\\Button.png");
    texture->SetSlice9Rect({10.f, 10.f, 44.f, 44.f});
    
    Button* login_button = canvas->AddWidget<Button>(L"Login Button");
    login_button->AttachToWidget(canvas->GetRootWidget());
    login_button->SetTexture(texture);
    login_button->SetDrawMode(DrawMode::kSliced);
    login_button->OnMouseReleased.Add([](Widget* kWidget)
    {
        World::Get()->OpenLevel(LevelType::kDefault);
    });

    Text* login_text = canvas->AddWidget<Text>(L"Login Text");
    login_text->AttachToWidget(login_button);
    login_text->SetAnchorPreset(AnchorPreset::kStretch);
    login_text->SetSize({0.f, 0.f});
    login_text->SetColor(Math::Color::Black);
    login_text->SetText(L"LOGIN");
    login_text->SetAlignment(Text::kMiddleCenter);

    Button* exit_button = canvas->AddWidget<Button>(L"Exit Button");
    exit_button->AttachToWidget(canvas->GetRootWidget());
    exit_button->SetAnchoredPosition({0.f, 50.f});
    exit_button->SetTexture(texture);
    exit_button->SetDrawMode(DrawMode::kSliced);
    exit_button->OnMouseReleased.Add([](Widget* kWidget)
    {
        WindowsWindow* window = World::Get()->GetWindow();
        PostMessage(window->GetHWnd(), WM_USER, 0, 0);
    });

    Text* exit_text = canvas->AddWidget<Text>(L"Exit Text");
    exit_text->AttachToWidget(exit_button);
    exit_text->SetAnchorPreset(AnchorPreset::kStretch);
    exit_text->SetSize({0.f, 0.f});
    exit_text->SetColor(Math::Color::Black);
    exit_text->SetText(L"EXIT");
    exit_text->SetAlignment(Text::kMiddleCenter);
    
}

RTTR_REGISTRATION
{
    rttr::registration::class_<MainMenu>("MainMenu")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
