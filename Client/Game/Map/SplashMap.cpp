#include "pch.h"
#include "SplashMap.h"

#include "Level/World.h"
#include "Resource/ResourceManager.h"
#include "Time/TimerManager.h"
#include "UI/Canvas.h"
#include "UI/Widget/Image.h"
#include "Windows/DX/UITexture.h"

SplashMap::SplashMap(const std::wstring& kName)
    : Level(kName)
{
}

void SplashMap::Load()
{
    ResourceManager::Get()->Load<UITexture>(L"FMODLogo", L".\\Game_Data\\FMODLogo.png");
    UITexture* texture = ResourceManager::Get()->GetResource<UITexture>(L"FMODLogo");
    
    Canvas* canvas = Canvas::Get();

    UI::Image* image = canvas->AddWidget<UI::Image>(L"FMODLogo");
    image->SetSize({texture->GetWidth() / 2.f, texture->GetHeight() / 2.f});
    image->SetAnchorPreset(UI::AnchorPresets::kMiddle | UI::AnchorPresets::kCenter, true);
    image->SetTexture(texture);

    TimerManager::Get()->SetTimer([]()
    {
        World::Get()->OpenLevel(LevelType::kMainMenu);
    }, 2.f);
}

RTTR_REGISTRATION
{
    rttr::registration::class_<SplashMap>("SplashMap")
        .constructor<const std::wstring&>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        );
}
