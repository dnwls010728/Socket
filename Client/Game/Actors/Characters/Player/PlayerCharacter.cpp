#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Components/PlayerController.h"
#include "Level/CameraManager.h"
#include "UI/UIManager.h"
#include "UI/Widget/TextBox.h"
#include "Windows/DX/Renderer.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    move_speed_(3.f),
    nickname_text_box_(nullptr)
{
    controller_ = AddComponent<PlayerController>(L"PlayerController");
    
}

void PlayerCharacter::OnAttack()
{
}

void PlayerCharacter::OnMovement()
{
    rigid_body_->SetLinearVelocity(controller_->GetMovementInput() * move_speed_);
}

void PlayerCharacter::ClearLinearVelocity()
{
    rigid_body_->SetLinearVelocity(Math::Vector2::Zero());
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    CameraManager* camera = CameraManager::Get();
    camera->SetSize(9.375f);
    camera->SetTarget(GetSharedThis());

    nickname_text_box_ = UI::TextBox::Create(L"Nickname");
    nickname_text_box_->SetSize({ 100.f, 50.f });
    nickname_text_box_->SetText(L"Player");

    UI::Manager::Get()->AddToViewport(nickname_text_box_);

    // TimerManager::Get()->SetTimer(timer_handle_, [&]()
    // {
    //     if (UI::Manager::Get()->IsInViewport(nickname_text_box_))
    //     {
    //         UI::Manager::Get()->RemoveFromViewport(nickname_text_box_);
    //     }
    //     else
    //     {
    //         UI::Manager::Get()->AddToViewport(nickname_text_box_);
    //     }
    // }, 1.f, true);
}

void PlayerCharacter::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);

    Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(GetTransform()->GetPosition());
    nickname_text_box_->SetPosition(screen_position + Math::Vector2::Up() * 32.f);
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
