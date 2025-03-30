#include "pch.h"
#include "BombBase.h"

#include "BoomBase.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

BombBase::BombBase(const std::wstring& kName) :
    Actor(kName),
    explosion_time_(2.f),
    timer_handle_()
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Bombs\\Bomb.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Bomb_0");
    }
}

void BombBase::BeginPlay()
{
    Actor::BeginPlay();

    TimerManager::Get()->SetTimer(timer_handle_, this, &BombBase::OnBoom, explosion_time_, false);
}

void BombBase::OnBoom()
{
    Logger::Print(L"Boom!");

    SpawnActor<Actor>(BoomBase::StaticClass(), L"Boom");

    Actor* up = SpawnActor<Actor>(BoomBase::StaticClass(), L"Boom");
    up->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Up());

    Actor* down = SpawnActor<Actor>(BoomBase::StaticClass(), L"Boom");
    down->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Down());

    Actor* left = SpawnActor<Actor>(BoomBase::StaticClass(), L"Boom");
    left->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Left());

    Actor* right = SpawnActor<Actor>(BoomBase::StaticClass(), L"Boom");
    right->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Right());
    
    Destroy();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<BombBase>("BombBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
