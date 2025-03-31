#include "pch.h"
#include "BoomBase.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

BoomBase::BoomBase(const std::wstring& kName) :
    Actor(kName)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    collider_ = AddComponent<BoxColliderComponent>(L"Collider");
    collider_->SetSize({1.f, 1.f});
    collider_->SetTrigger(true);
    
    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Bombs\\Boom.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Boom_0");
    }
}

void BoomBase::BeginPlay()
{
    Actor::BeginPlay();

    SetLifeSpan(1.f);
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<BoomBase>("BoomBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
