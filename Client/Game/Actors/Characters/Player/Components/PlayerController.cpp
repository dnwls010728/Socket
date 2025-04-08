#include "pch.h"
#include "PlayerController.h"

#include "DebugDrawHelper.h"
#include "Actor/Component/TransformComponent.h"
#include "Actors/Dummy.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Actors/Characters/Player/States/PlayerDashState.h"
#include "Actors/Characters/Player/States/PlayerIdleState.h"
#include "Actors/Characters/Player/States/PlayerWalkState.h"
#include "Actors/Components/StateMachineComponent.h"
#include "DirectXTK/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "FSM/Condition.h"
#include "Math/Math.h"
#include "Physics/Physics2D.h"
#include "Windows/DX/Renderer.h"

PlayerController::PlayerController(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    character_(nullptr),
    movement_input_(Math::Vector2::Zero()),
    mouse_direction_(Math::Vector2::Zero()),
    mouse_direction_angle_(0.f),
    mouse_distance_(0.f),
    interaction_timer_()
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
            std::shared_ptr<PlayerDashState> dash_state = std::make_shared<PlayerDashState>(character_->GetSharedThis());

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
    
    TimerManager::Get()->SetTimer(interaction_timer_, this, &PlayerController::UpdateInteraction, 1.f / 60.f, true);

}

void PlayerController::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    Math::Vector2 character_position = Math::Vector2::Zero();

    if (IsValid(character_))
    {
        character_position = character_->GetTransform()->GetPosition();
    }
    
    if (Keyboard* keyboard = Keyboard::Get())
    {
        movement_input_.x = keyboard->GetKey('D') - keyboard->GetKey('A');
        movement_input_.y = keyboard->GetKey('W') - keyboard->GetKey('S');
    }

    if (Mouse* mouse = Mouse::Get())
    {
        Math::Vector2 mouse_position = Renderer::Get()->ConvertScreenToWorld(mouse->GetMousePosition());
        mouse_direction_ = (mouse_position - character_position).Normalized();
        mouse_distance_ = Math::Vector2::Distance(mouse_position, character_position);

        float theta = std::atan2f(mouse_direction_.x, mouse_direction_.y);
        mouse_direction_angle_ = theta * Math::Rad2Deg();
        
        if (mouse->GetMouseButtonDown(MouseButton::kLeft))
        {
            if (IsValid(character_)) character_->OnAttack();
        }
    }

    // 범위 디버그
    DebugDrawHelper::Get()->DrawRay(character_position, mouse_direction_ * 5.f, Math::Color::Green);
    
    float angle = std::atan2f(mouse_direction_.y, mouse_direction_.x);
    float new_angle = angle - 22.5f * Math::Deg2Rad();
    DebugDrawHelper::Get()->DrawRay(character_position, Math::Vector2(std::cos(new_angle), std::sin(new_angle)) * 5.f, Math::Color::Green);
    new_angle = angle + 22.5f * Math::Deg2Rad();
    DebugDrawHelper::Get()->DrawRay(character_position, Math::Vector2(std::cos(new_angle), std::sin(new_angle)) * 5.f, Math::Color::Green);
    
}

void PlayerController::UpdateInteraction()
{
    Math::Vector2 character_position = Math::Vector2::Zero();
    if (IsValid(character_))
    {
        character_position = character_->GetTransform()->GetPosition();

        std::vector<Actor*> out_actors;
        if (Physics2D::OverlapCircleAll(character_position, 5.f, out_actors))
        {
            for (const auto& kActor: out_actors)
            {
                if (kActor == character_) continue;
                
                Math::Vector2 actor_direction = (kActor->GetTransform()->GetPosition() - character_position).Normalized();

                float dot = Math::Vector2::Dot(mouse_direction_, actor_direction);
                float radian = std::acosf(dot);
                float degree = radian * Math::Rad2Deg();

                if (degree < 45.f)
                {
                    DebugDrawHelper::Get()->DrawCircle(kActor->GetTransform()->GetPosition(), 1.f, Math::Color::Red);
                    Dummy* dummy = dynamic_cast<Dummy*>(kActor);
                    dummy->Show();
                }
                else
                {
                    Dummy* dummy = dynamic_cast<Dummy*>(kActor);
                    dummy->Hide();
                }
            }
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
