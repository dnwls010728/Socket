#include "pch.h"
#include "Platform.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "PlatformControllerComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Resource/ResourceManager.h"
#include "rttr/registration.h"
#include "rttr/detail/registration/registration_impl.h"
#include "Windows/DX/Sprite.h"

Platform::Platform(const std::wstring& kName) :
    Actor(kName)
{
    SetLayer(ActorLayer::kGround);

    GetTransform()->SetScale({2.f, 1.f});

    renderer_ = AddComponent<SpriteRendererComponent>(L"Renderer");
    
    if (ResourceManager::Get()->Load<Sprite>(L"Square", L".\\Game_Data\\Default\\Square.png"))
    {
        sprite_ = ResourceManager::Get()->GetResource<Sprite>(L"Square");
        sprite_->SetPPU(256);
    }

    sprite_->Split(1, 1, Sprite::kCenter);
    sprite_->SetFilterMode(FilterMode::kBilinear);

    renderer_->SetSprite(sprite_);
    
    collider_ = AddComponent<BoxColliderComponent>(L"Collider");
    collider_->SetSize({2.f, 1.f});
    
    controller_ = AddComponent<PlatformControllerComponent>(L"Controller");
}

void Platform::Tick(float delta_time)
{
    Actor::Tick(delta_time);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Platform>("Platform")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
