#include "pch.h"
#include "PlayerCharacter.h"

#include "Weapon.h"
#include "../../CommonDLL/Packet.h"
#include "../../CommonDLL/SendBuffer.h"
#include "../SocketCore/ServerPacketHandler.h"
#include "Actor/Camera.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "State/PlayerLocomotion.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    horizontal_axis_(0),
    move_speed_(2.f),
    previous_position_(Math::Vector2::Zero()),
    weapon_(nullptr)
{
    SetLayer(ActorLayer::kPlayer);

    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});
    
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Character\\Player\\PlayerSheet.png.animpack");
    
    animator_ = AddComponent<AnimatorComponent>(L"Animator");
    animator_->SetAnimationPack(animation_pack_);

    state_machine_ = AddComponent<StateMachine>(L"StateMachine");
    
    locomotion_state_ = std::make_shared<PlayerLocomotion>(state_machine_);
    state_machine_->ChangeState(locomotion_state_.get());
    
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    if (is_mine_)
    {
        Camera::Get()->SetTarget(this);

        weapon_ = World::Get()->SpawnActor<Weapon>(Weapon::StaticClass(), L"Weapon");
        if (IsValid(weapon_)) weapon_->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Up() * .4f);
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
        horizontal_axis_ = keyboard->GetKey('D') - keyboard->GetKey('A');
        
        if (keyboard->GetKeyDown(VK_SPACE))
        {
            rigid_body_->AddForceY(7.f, ForceMode::kImpulse);
        }

        Math::Vector2 position = GetTransform()->GetPosition();

        Mouse* mouse = Mouse::Get();
        Math::Vector2 mouse_position = Renderer::Get()->ConvertScreenToWorld(mouse->GetMousePosition());
        Math::Vector2 direction = (mouse_position - position).Normalized();
        
        renderer_->SetFlipX(direction.x < 0);

        static float send_timer = 0.f;
        send_timer += delta_time;

        // 1초에 10번 동기화
        if (send_timer > 1.f / 10.f)
        {
            send_timer = 0.f;
            
            C_Moving pkt;
            pkt._locationX = transform_->GetPosition().x;
            pkt._locationY = transform_->GetPosition().y;
            std::shared_ptr<SendBuffer> sendBuffer = ServerPacketHandler::MakeSendBuffer<C_Moving>(pkt,C_PKT_MOVING);
            GSocketSession->Send(sendBuffer);
        }
    }
    else
    {
        Math::Vector2 position = Math::Vector2::Lerp(GetTransform()->GetPosition(), received_position_, delta_time * 10.f);
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
    
    if (IsValid(weapon_)) weapon_->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Up() * .4f);
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
