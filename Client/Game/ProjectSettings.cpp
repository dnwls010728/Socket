#include "pch.h"
#include "ProjectSettings.h"

#include "Maps/LoginMap.h"
#include "Maps/InGameMap.h"

ProjectSettings::ProjectSettings()
{
}

void ProjectSettings::Init()
{
    EngineSettings* settings = EngineSettings::Get();
    if (!settings) return;

    // 윈도우 설정
    settings->SetWindowTitle(L"Untitled");
    settings->SetScreenSize(800, 600);
    settings->UseVSync(false);

    // 물리 설정
    settings->SetFixedTimeStep(1.f / 60.f);

    // 레이어 충돌 매트릭스 설정
    settings->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault | ActorLayer::kPlayer);
    settings->AddCollisionLayer(ActorLayer::kPlayer, ActorLayer::kDefault);

    settings->AddLevel<LoginMap>(L"Login");
    settings->AddLevel<InGameMap>(L"InGame");

    settings->SetDefaultLevel(L"Login");
    
}
