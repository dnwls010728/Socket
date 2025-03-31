#include "pch.h"
#include "BoomBase.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

BoomBase::BoomBase(const std::wstring& kName) :
    Actor(kName),
    direction_(Math::Vector2::Zero()),
    length_(0)
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

    TimerManager::Get()->SetTimer(timer_handle_, this, &BoomBase::OnBoom, .1f, false);
    SetLifeSpan(.5f);

}

void BoomBase::OnBoom()
{
    if (direction_ != Math::Vector2::Zero() && length_ > 0)
    {
        BoomBase* boom = SpawnActor<BoomBase>(BoomBase::StaticClass(), L"Bomb");
        boom->GetTransform()->SetPosition(GetTransform()->GetPosition() + direction_);
        boom->SetDirection(direction_);
        boom->SetLength(--length_);
    }
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
