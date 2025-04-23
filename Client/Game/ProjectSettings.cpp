#include "pch.h"
#include "ProjectSettings.h"

#include "Maps/Map_Login.h"
#include "Maps/Map_Begin.h"

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
    settings->SetFixedTimeStep(.02f);

    // 레이어 충돌 매트릭스 설정
    settings->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault | ActorLayer::kPlayer);
    settings->AddCollisionLayer(ActorLayer::kPlayer, ActorLayer::kDefault);

    settings->AddLevel<Map_Login>(L"Login");
    settings->AddLevel<Map_Begin>(L"0");
    settings->AddLevel<Map_Begin>(L"1");

    settings->SetDefaultLevel(L"Login");
    
}
