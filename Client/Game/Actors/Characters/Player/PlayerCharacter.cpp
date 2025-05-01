#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actors/Characters/Components/Controller2DComponent.h"
#include "Asset/AssetManager.h"
#include "Components/InventoryComponent.h"
#include "Input/Keyboard.h"
#include "Math/Math.h"
#include "Subsystems/NetworkSubsystem.h"
#include "UI/ChatBalloon.h"
#include "UI/MiniMap.h"
#include "UI/UIManager.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    movement_input_(Math::Vector2::Zero()),
    last_movement_(),
    movements_(),
    is_jump_(false),
    timer_(0),
    chat_balloon_(nullptr),
    chat_balloon_timer_handle_()
{
    inventory_ = AddComponent<InventoryComponent>(L"Inventory");
    
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

void PlayerCharacter::InitSpawn(const Math::Vector2& position)
{
    GetTransform()->SetPosition(position);
    last_movement_.x = position.x;
    last_movement_.y = position.y;
}

void PlayerCharacter::Speak(const std::wstring& message)
{
    UI::Manager* ui_manager = UI::Manager::Get();
    TimerManager* timer_manager = TimerManager::Get();

    chat_balloon_->SetText(message);
    
    if (ui_manager->IsInViewport(chat_balloon_))
    {
        if (chat_balloon_timer_handle_.IsValid())
            timer_manager->ClearTimer(chat_balloon_timer_handle_);
    }
    else
    {
        Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(GetTransform()->GetPosition());
        chat_balloon_->SetPosition(screen_position + Math::Vector2::Down() * 40.f);
        
        ui_manager->AddToViewport(chat_balloon_);
    }

    timer_manager->SetTimer(chat_balloon_timer_handle_, [&]()
    {
        UI::Manager::Get()->RemoveFromViewport(chat_balloon_);
    }, 4.f, false);
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Box.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Box_0");
    }

    chat_balloon_ = UI::ChatBalloon::Create(L"ChatBalloon");
    chat_balloon_->SetSize({8.f, 8.f});
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);
    
    std::shared_ptr<TransformComponent> transform = GetTransform();

    if (IsMine())
    {
        const Controller2DComponent::CollisionInfo& collisions = controller_->GetCollisions();
        if (collisions.is_above || collisions.is_below) velocity_.y = 0.f;

        if (is_jump_ && collisions.is_below)
        {
            velocity_.y = 10.f;
            is_jump_ = false;
        }
        
        velocity_.x = movement_input_.x * 5.f;
        velocity_.y += gravity_ * delta_time;
        controller_->Move(velocity_ * delta_time);
        
        Math::Vector2 position = transform->GetPosition();
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
        // 50 프레임 딜레이
        if (timer_ > 1) timer_--;
        else if (timer_ == 1)
        {
            if (!movements_.empty())
            {
                last_movement_ = movements_.front();
                movements_.pop();
            }
            else timer_ = 0;
        }
        
        Math::Vector2 position = transform->GetPosition();
        float x_speed = last_movement_.x - position.x;
        float y_speed = last_movement_.y - position.y;
        transform->Translate({x_speed, y_speed});
    }

    UI::Manager* ui_manager = UI::Manager::Get();
    if (ui_manager->IsInViewport(chat_balloon_))
    {
        Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(transform->GetPosition());
        chat_balloon_->SetPosition(screen_position + Math::Vector2::Down() * 40.f);
    }
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    if (IsMine())
    {
        Keyboard* keyboard = Keyboard::Get();
        if (!UI::Manager::Get()->HasFocus())
        {
            movement_input_.x = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
            movement_input_.y = keyboard->GetKey(VK_UP) - keyboard->GetKey(VK_DOWN);

            if (keyboard->GetKeyDown(VK_SPACE))
            {
                is_jump_ = true;
            }

            if (keyboard->GetKeyDown('1'))
            {
                GET_NETWORK()->ChangeMap(0);
            }

            if (keyboard->GetKeyDown('2'))
            {
                GET_NETWORK()->ChangeMap(1);
            }
        }
        else
        {
            movement_input_.x = 0.f;
            movement_input_.y = 0.f;
        }
    }
    else
    {
    }
}

void PlayerCharacter::EndPlay(EndPlayReason type)
{
    CharacterBase::EndPlay(type);

    UI::Manager* ui_manager = UI::Manager::Get();
    ui_manager->RemoveFromViewport(chat_balloon_);
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
