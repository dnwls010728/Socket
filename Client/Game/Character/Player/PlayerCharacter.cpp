#include "pch.h"
#include "PlayerCharacter.h"

#include "DebugDrawHelper.h"
#include "Logger.h"
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
#include "Audio/Audio.h"
#include "Audio/AudioManager.h"
#include "Character/Component/FSM/StateMachine.h"
#include "Data/CSVReader.h"
#include "Data/WeaponData.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Math/Math.h"
#include "State/PlayerIdle.h"
#include "State/PlayerWalk.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    h_axis_(0),
    move_speed_(2.f),
    previous_position_(Math::Vector2::Zero()),
    weapon_(nullptr),
    timer_handle_(),
    shot_direction_(Math::Vector2::Zero())
{
    AssetManager* asset_manager = AssetManager::Get();
    sprite_ = asset_manager->Load<Sprite>(L"Sprites\\Character\\Player\\PlayerSheet.png");
    animation_pack_ = asset_manager->Load<AnimationPack>(L"Sprites\\Character\\Player\\PlayerSheet.png.animpack");
    audio_ = asset_manager->Load<Audio>(L"Audio\\SE\\GUNSupr_Silenced Pistol Fire Short_01.wav");
    
    SetLayer(ActorLayer::kPlayer);

    renderer_->SetSprite(sprite_, L"PlayerSheet_0");

    collider_->SetOffset({0.f, .5f});
    collider_->SetSize({.5f, .5f});
    
    animator_->SetAnimationPack(animation_pack_);

    {
        idle_state_ = std::make_shared<PlayerIdle>(state_machine_);
        walk_state_ = std::make_shared<PlayerWalk>(state_machine_);
    }

    state_machine_->ChangeState(idle_state_.get());
    
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();
    
    Camera::Get()->SetTarget(this);
    
    weapon_ = World::Get()->SpawnActor<Weapon>(Weapon::StaticClass(), L"Weapon");
    if (IsValid(weapon_))
    {
        weapon_->SetOwner(this);
        weapon_->GetTransform()->SetPosition(GetTransform()->GetPosition() + Math::Vector2::Up() * .4f);
    }

    std::vector<WeaponData> weapon_data;
    CSVReader::Parse(L"Data\\WeaponData.csv", weapon_data);
    
}

void PlayerCharacter::EndPlay(EndPlayReason type)
{
    CharacterBase::EndPlay(type);

    if (type == EndPlayReason::kDestroyed)
    {
        if (IsValid(weapon_)) weapon_->Destroy();
    }
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);
    
    rigid_body_->SetLinearVelocityX(h_axis_ * move_speed_);
    
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    Keyboard* keyboard = Keyboard::Get();
    h_axis_ = keyboard->GetKey('D') - keyboard->GetKey('A');
        
    if (keyboard->GetKeyDown(VK_SPACE))
    {
        rigid_body_->AddForceY(7.f, ForceMode::kImpulse);
    }

    Math::Vector2 position = GetTransform()->GetPosition() + Math::Vector2::Up() * .4f;

    Mouse* mouse = Mouse::Get();
    Math::Vector2 mouse_position = Renderer::Get()->ConvertScreenToWorld(mouse->GetMousePosition());
    Math::Vector2 direction = (mouse_position - position).Normalized();
        
    renderer_->SetFlipX(direction.x < 0);

    float length = (position - mouse_position).Magnitude();
    DebugDrawHelper::Get()->DrawSegment(position, position + (length * direction), Math::Color::Red);

    if (IsValid(weapon_))
    {
        Math::Vector2 offset = direction * .2f;
        Math::Vector2 new_position = position + offset;
            
        weapon_->GetRenderer()->SetFlipX(direction.x < 0);
            
        float theta = std::atan2f(direction.y, direction.x);
    
        float degree;
        if (direction.x < 0.f) degree = theta * Math::Rad2Deg() - 135.f;
        else degree = theta * Math::Rad2Deg() - 45.f;

        weapon_->GetTransform()->SetPosition(new_position);
        weapon_->GetTransform()->SetAngle(degree);

        shot_direction_ = direction;

        if (mouse->GetMouseButtonDown(MouseButton::kLeft))
        {
            TimerManager::Get()->SetTimer(timer_handle_, [&]()
            {
                AudioManager::Get()->PlayOneShot(audio_);
                weapon_->Shot(shot_direction_);
            }, .1f, true);
        }

        if (mouse->GetMouseButtonUp(MouseButton::kLeft))
        {
            TimerManager::Get()->ClearTimer(timer_handle_);
        }
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
