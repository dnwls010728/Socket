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
    Math::Vector2 position = GetTransform()->GetPosition();

    BoomBase* center = SpawnActor<BoomBase>(BoomBase::StaticClass(), L"Boom");
    center->GetTransform()->SetPosition(position);

    BoomBase* up = SpawnActor<BoomBase>(BoomBase::StaticClass(), L"Boom");
    up->GetTransform()->SetPosition(position + Math::Vector2::Up());
    up->SetDirection(Math::Vector2::Up());
    up->SetLength(5);

    BoomBase* down = SpawnActor<BoomBase>(BoomBase::StaticClass(), L"Boom");
    down->GetTransform()->SetPosition(position + Math::Vector2::Down());
    down->SetDirection(Math::Vector2::Down());
    down->SetLength(5);

    BoomBase* left = SpawnActor<BoomBase>(BoomBase::StaticClass(), L"Boom");
    left->GetTransform()->SetPosition(position + Math::Vector2::Left());
    left->SetDirection(Math::Vector2::Left());
    left->SetLength(5);

    BoomBase* right = SpawnActor<BoomBase>(BoomBase::StaticClass(), L"Boom");
    right->GetTransform()->SetPosition(position + Math::Vector2::Right());
    right->SetDirection(Math::Vector2::Right());
    right->SetLength(5);
    
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
