#include "pch.h"
#include "PlayerController.h"

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "DirectXTK/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

PlayerController::PlayerController(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    character_(nullptr),
    movement_input_(Math::Vector2::Zero())
{
}

void PlayerController::BeginPlay()
{
    ActorComponent::BeginPlay();

    character_ = dynamic_cast<PlayerCharacter*>(GetOwner());

}

void PlayerController::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    
    if (Keyboard* keyboard = Keyboard::Get())
    {
        movement_input_.x = keyboard->GetKey('D') - keyboard->GetKey('A');
        movement_input_.y = keyboard->GetKey('W') - keyboard->GetKey('S');

        if (movement_input_ != Math::Vector2::Zero())
        {
        }
    }

    if (Mouse* mouse = Mouse::Get())
    {
        if (mouse->GetMouseButtonDown(MouseButton::kLeft))
        {
            if (character_) character_->OnAttack();
        }
    }
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerController>("PlayerController")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
