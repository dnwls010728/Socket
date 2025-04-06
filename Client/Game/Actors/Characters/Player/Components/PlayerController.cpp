#include "pch.h"
#include "PlayerController.h"

#include "Input/Keyboard.h"

PlayerController::PlayerController(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    movement_input_(Math::Vector2::Zero())
{
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
