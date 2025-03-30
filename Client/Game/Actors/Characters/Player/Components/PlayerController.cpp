#include "pch.h"
#include "PlayerController.h"

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Input/Keyboard.h"

PlayerController::PlayerController(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    movement_input_(Math::Vector2::Zero()),
    player_character_(nullptr)
{
}

void PlayerController::BeginPlay()
{
    ActorComponent::BeginPlay();

    player_character_ = dynamic_cast<PlayerCharacter*>(GetOwner());
}

void PlayerController::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    
    if (Keyboard* keyboard = Keyboard::Get())
    {
        movement_input_.x = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
        movement_input_.y = keyboard->GetKey(VK_UP) - keyboard->GetKey(VK_DOWN);

        if (keyboard->GetKeyDown(VK_SPACE))
        {
            player_character_->SpawnBomb();
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
