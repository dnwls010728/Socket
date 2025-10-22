#include "pch.h"
#include "ProjectSettings.h"

#include "Maps/EditorMap.h"
#include "Maps/LoginMap.h"
#include "Maps/GameMap.h"

ProjectSettings::ProjectSettings()
{
}

void ProjectSettings::Init()
{
    EngineSettings* settings = EngineSettings::Get();
    if (!settings) return;

    // 윈도우 설정
    settings->SetWindowTitle(L"RpGB");
    settings->SetScreenSize(1366, 768);
    settings->UseVSync(false);

    // 물리 설정
    settings->SetFixedTimeStep(1.f / 60.f);

    // 보간 지연
    settings->SetObjectInterpolationDelay(0.13f);
    settings->SetCharacterInterpolationDelay(0.13f);
    
    // 레이어 충돌 매트릭스 설정
    settings->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault | ActorLayer::kPlayer | ActorLayer::kNPC | ActorLayer::kMob | ActorLayer::kDroppedItem);
    settings->AddCollisionLayer(ActorLayer::kPortal, ActorLayer::kDefault | ActorLayer::kPortal | ActorLayer::kPlayer | ActorLayer::kNPC | ActorLayer::kMob | ActorLayer::kDroppedItem);
    settings->AddCollisionLayer(ActorLayer::kPlayer, ActorLayer::kDefault);
    settings->AddCollisionLayer(ActorLayer::kNPC, ActorLayer::kDefault | ActorLayer::kPlayer);
    settings->AddCollisionLayer(ActorLayer::kMob, ActorLayer::kDefault);
    settings->AddCollisionLayer(ActorLayer::kDroppedItem, ActorLayer::kDefault);

    settings->AddLevel<EditorMap>(L"Editor");
    settings->AddLevel<LoginMap>(L"Login");
    settings->AddLevel<GameMap>(L"Game");

    // settings->SetDefaultLevel(L"Login");
    settings->SetDefaultLevel(L"Editor");
    
}
