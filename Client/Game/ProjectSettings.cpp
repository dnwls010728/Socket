#include "pch.h"
#include "ProjectSettings.h"

#include "Maps/EditorMap.h"
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

    // 보간 지연
    settings->SetInterpolationDelay(0.1f);
    
    // 레이어 충돌 매트릭스 설정
    settings->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault | ActorLayer::kCharacter);
    settings->AddCollisionLayer(ActorLayer::kCharacter, ActorLayer::kDefault);

    settings->AddLevel<EditorMap>(L"Editor");
    settings->AddLevel<LoginMap>(L"Login");
    settings->AddLevel<InGameMap>(L"InGame");

    settings->SetDefaultLevel(L"Login");
    // settings->SetDefaultLevel(L"Editor");
    
}
