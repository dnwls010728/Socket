#include "pch.h"
#include "PlayerCharacter.h"
#include "../../CommonDLL/Packet.h"
#include "../../CommonDLL/SendBuffer.h"
#include "../SocketCore/ServerPacketHandler.h"
#include "Actor/Camera.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Input/Keyboard.h"
#include "Resource/ResourceManager.h"
#include "Windows/DX/Sprite.h"

Type::uint32 current_player_id = 0;

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    horizontal_axis_(0),
    move_speed_(2.f),
    previous_position_(Math::Vector2::Zero()),
    last_recent_position_(Math::Vector2::Zero())
{
    SetLayer(ActorLayer::kPlayer);
    
    sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Character\\PlayerSheet.png");
    if (sprite_) renderer_->SetSprite(sprite_);

    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});
    
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    if (is_mine_)
    {
        Camera::Get()->SetTarget(this);
    }
    else
    {
        rigid_body_->SetBodyType(BodyType::kStatic);
    }
    
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);

    if (is_mine_)
    {
        if (horizontal_axis_ != 0)
        {
            renderer_->SetFlipX(horizontal_axis_ < 0);
            rigid_body_->SetLinearVelocityX(horizontal_axis_ * move_speed_);
        }
    }
    
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    if (is_mine_)
    {
        Keyboard* keyboard = Keyboard::Get();
        horizontal_axis_ = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
        
        if (keyboard->GetKeyDown('C'))
        {
            rigid_body_->AddForceY(7.f, ForceMode::kImpulse);
        }

        C_MovingPacket pkt;
        pkt._locationX = transform_->GetPosition().x;
        pkt._locationY = transform_->GetPosition().y;
        std::shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer<C_MovingPacket>(pkt,C_PKT_MOVING);
        GSocketSession->Send(sendBuffer);
    }
    else
    {
        Math::Vector2 position = Math::Vector2::Lerp(GetTransform()->GetPosition(), last_recent_position_, delta_time * 100.f);
        transform_->SetPosition(position);
    }
}
void PlayerCharacter::PostTick(float delta_time)
{
    CharacterBase::PostTick(delta_time);

    TransformComponent* transform = GetTransform();
    Math::Vector2 position = transform->GetPosition();
    Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(position);
    
    // nickname_text_->SetPosition(screen_position);
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
