#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Input/Keyboard.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    movement_input_(Math::Vector2::Zero())
{
    SetLayer(ActorLayer::kPlayer);
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Capsule.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Capsule_0");
    }

    if (IsOwner())
    {
        renderer_->SetColor(Math::Color::Green);

        rigid_body_->SetBodyType(BodyType::kDynamic);
    }
    else
    {
        renderer_->SetColor(Math::Color::Red);
    }
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);

    if (IsOwner())
    {
        rigid_body_->SetLinearVelocityX(movement_input_.x * 5.f);
    }
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    std::shared_ptr<TransformComponent> transform = GetTransform();
    Math::Vector2 position = transform->GetPosition();

    if (IsOwner())
    {
        Keyboard* keyboard = Keyboard::Get();
        movement_input_.x = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
        movement_input_.y = keyboard->GetKey(VK_UP) - keyboard->GetKey(VK_DOWN);

        if (keyboard->GetKeyDown(VK_SPACE))
        {
            rigid_body_->AddForceY(10.f, ForceMode::kImpulse);
        }

        MovePlayerPacket packet;
        packet.x = position.x;
        packet.y = position.y;
        SendPacket(packet);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerCharacter>("PlayerCharacter")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
