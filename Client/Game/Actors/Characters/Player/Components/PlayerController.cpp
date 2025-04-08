#include "pch.h"
#include "PlayerController.h"

#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Characters/Player/States/PlayerIdleState.h"
#include "Actors/Characters/Player/States/PlayerWalkState.h"
#include "Actors/Components/StateMachineComponent.h"
#include "DirectXTK/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "FSM/Condition.h"

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
    if (character_)
    {
        std::shared_ptr<StateMachineComponent> state_machine = character_->GetStateMachine();
        if (state_machine)
        {
            state_machine_ = state_machine;

            std::shared_ptr<PlayerIdleState> idle_state = std::make_shared<PlayerIdleState>(character_->GetSharedThis());
            std::shared_ptr<PlayerWalkState> walk_state = std::make_shared<PlayerWalkState>(character_->GetSharedThis());

            state_machine->AddTransition(idle_state, walk_state, [&]()
            {
                return movement_input_.Magnitude() > 0.f;
            });

            state_machine->AddTransition(walk_state, idle_state, [&]()
            {
                return movement_input_.Magnitude() == 0.f;
            });

            state_machine->SetState(idle_state);
        }
    }

}

void PlayerController::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    
    if (Keyboard* keyboard = Keyboard::Get())
    {
        movement_input_.x = keyboard->GetKey('D') - keyboard->GetKey('A');
        movement_input_.y = keyboard->GetKey('W') - keyboard->GetKey('S');
    }

    if (Mouse* mouse = Mouse::Get())
    {
        if (mouse->GetMouseButtonDown(MouseButton::kLeft))
        {
            if (IsValid(character_)) character_->OnAttack();
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
