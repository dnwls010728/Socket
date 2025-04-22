#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Input/Keyboard.h"
#include "Math/Math.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    movement_input_(Math::Vector2::Zero()),
    last_movement_(),
    movements_(),
    timer_(0),
    is_jump_(false)
{
    SetLayer(ActorLayer::kPlayer);
}

void PlayerCharacter::ReceivePacket(Net::IPacket* packet)
{
    CharacterBase::ReceivePacket(packet);

    switch (packet->GetPacketID())
    {
    case MovePlayerPacket::StaticPacketID:
        {
            MovePlayerPacket* move_player_packet = static_cast<MovePlayerPacket*>(packet);
            movements_.push(move_player_packet->movement);

            if (timer_ == 0) timer_ = 50;
        }
        break;
        
    default:
        break;
    }
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Capsule.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Capsule_0");
    }

    if (IsMine())
    {
        renderer_->SetColor(Math::Color::Green);

        rigid_body_->SetBodyType(BodyType::kDynamic);
    }
    else
    {
        renderer_->SetColor(Math::Color::Red);
        
        rigid_body_->SetBodyType(BodyType::kDynamic);
    }
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);

    Math::Vector2 position = rigid_body_->GetPosition();

    if (IsMine())
    {
        rigid_body_->SetLinearVelocityX(movement_input_.x * 5.f);

        if (is_jump_)
        {
            is_jump_ = false;
            rigid_body_->AddForceY(10.f, ForceMode::kImpulse);
        }
        
        Movement movement = {position.x, position.y};
        if (!Math::IsEqual(last_movement_.x, movement.x) || !Math::IsEqual(last_movement_.y, movement.y))
        {
            MovePlayerPacket move_player_packet;
            move_player_packet.movement = movement;
            SendPacket(move_player_packet);
            
            last_movement_ = movement;
        }
    }
    else
    {
        if (timer_ > 1)
        {
            timer_--;
        }
        else if (timer_ == 1)
        {
            if (!movements_.empty())
            {
                last_movement_ = movements_.front();
                movements_.pop();
            }
            else timer_ = 0;
        }

        float x_speed = last_movement_.x - position.x;
        float y_speed = last_movement_.y - position.y;
        rigid_body_->SetLinearVelocity({x_speed, y_speed});
    }
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    if (IsMine())
    {
        Keyboard* keyboard = Keyboard::Get();
        movement_input_.x = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
        movement_input_.y = keyboard->GetKey(VK_UP) - keyboard->GetKey(VK_DOWN);

        if (keyboard->GetKeyDown(VK_SPACE))
        {
            is_jump_ = true;
        }
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
