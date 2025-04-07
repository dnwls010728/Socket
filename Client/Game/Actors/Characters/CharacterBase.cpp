#include "pch.h"
#include "CharacterBase.h"

#include "DebugDrawHelper.h"
#include "Actor/Component/CircleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName)
{
    collider_ = AddComponent<CircleColliderComponent>(L"CircleCollider");
    collider_->SetRadius(.5f);
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");

}

void CharacterBase::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    Math::Vector2 position = GetTransform()->GetPosition();
    DebugDrawHelper::Get()->DrawBox(position, { 1.0f, 1.0f }, Math::Color::White);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CharacterBase>("CharacterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
