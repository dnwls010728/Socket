#include "pch.h"
#include "ProjectSettings.h"

ProjectSettings::ProjectSettings()
{
}

void ProjectSettings::Init()
{
    EngineSettings* settings = EngineSettings::Get();
    if (!settings) return;

    // 윈도우 설정
    settings->SetWindowTitle(L"Untitled");
    settings->SetScreenSize(640, 480);
    settings->UseVSync(false);

    // 물리 설정
    settings->SetFixedTimeStep(.02f);

    // 레이어 충돌 매트릭스 설정
    settings->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault);
    
}
