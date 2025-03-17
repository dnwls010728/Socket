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
    settings->AddCollisionLayer(ActorLayer::kDefault, ActorLayer::kDefault | ActorLayer::kGround | ActorLayer::kPlayer | ActorLayer::kMob | ActorLayer::kBullet | ActorLayer::kBlock);
    settings->AddCollisionLayer(ActorLayer::kGround, ActorLayer::kGround | ActorLayer::kDefault | ActorLayer::kPlayer | ActorLayer::kMob | ActorLayer::kBullet | ActorLayer::kBlock);
    settings->AddCollisionLayer(ActorLayer::kPlayer, ActorLayer::kGround | ActorLayer::kDefault | ActorLayer::kBlock);
    settings->AddCollisionLayer(ActorLayer::kMob, ActorLayer::kBullet | ActorLayer::kGround | ActorLayer::kDefault | ActorLayer::kBlock);
    settings->AddCollisionLayer(ActorLayer::kBullet, ActorLayer::kMob | ActorLayer::kBlock);
    settings->AddCollisionLayer(ActorLayer::kBlock, ActorLayer::kDefault | ActorLayer::kGround | ActorLayer::kPlayer | ActorLayer::kMob | ActorLayer::kBullet);
    
}
