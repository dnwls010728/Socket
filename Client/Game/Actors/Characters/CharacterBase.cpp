#include "pch.h"
#include "CharacterBase.h"

#include "DebugDrawHelper.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Components/Controller2DComponent.h"
#include "UI/ChatBalloon.h"
#include "UI/UIInGameState.h"
#include "UI/UIManager.h"
#include "UI/Element/UINameTag.h"
#include "Windows/DX/Sprite.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    NetworkActor(kName),
    character_name_(L"Unknown"),
    state_machine_(nullptr),
    velocity_(Math::Vector2::Zero()),
    gravity_(-20.f),
    name_tag_(nullptr),
    chat_balloon_(nullptr),
    chat_balloon_timer_handle_()
{
    collider_ = AddComponent<BoxColliderComponent>(L"BoxCollider");
    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(std::numeric_limits<int32_t>::max());

    animator_ = AddComponent<AnimatorComponent>(L"Animator");
    state_machine_ = AddComponent<StateMachineComponent>(L"StateMachine");

}

void CharacterBase::SetCharacterName(const std::wstring& name)
{
    character_name_ = name;
    
    if (HasBegunPlay())
    {
        if (name_tag_) name_tag_->SetText(character_name_);
    }
}

void CharacterBase::Speak(const std::wstring& message, float duration)
{
    TimerManager* timer_manager = TimerManager::Get();

    chat_balloon_->SetText(message);
    
    if (chat_balloon_->IsInViewport())
    {
        if (chat_balloon_timer_handle_.IsValid())
            timer_manager->ClearTimer(chat_balloon_timer_handle_);
    }
    else chat_balloon_->AddToViewport();

    timer_manager->SetTimer(chat_balloon_timer_handle_, this, &CharacterBase::OnSpeakEnd, duration, false);
}

void CharacterBase::BeginPlay()
{
    NetworkActor::BeginPlay();

    if (auto state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        name_tag_ = state->AddElement<UINameTag>(UINameTag::StaticClass(), L"NameTag");
        name_tag_->SetText(character_name_);
    }
    
    chat_balloon_ = UI_OLD::ChatBalloon::Create(L"ChatBalloon");
    chat_balloon_->SetSize({8.f, 8.f});
}

void CharacterBase::PhysicsTick(float delta_time)
{
    NetworkActor::PhysicsTick(delta_time);
    
    Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(GetTransform()->GetPosition());

    Math::Vector2 name_tag_offset = { -name_tag_->GetSize().x * .5f, 4.f };
    name_tag_->SetAbsolutePosition(screen_position + name_tag_offset);
    
    if (chat_balloon_->IsInViewport())
        chat_balloon_->SetPosition(screen_position + Math::Vector2::Down() * 70.f);
}

void CharacterBase::EndPlay(EndPlayReason type)
{
    NetworkActor::EndPlay(type);

    if (auto state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->RemoveElement(name_tag_);
    }
    
    TimerManager::Get()->ClearTimer(chat_balloon_timer_handle_);
    
    chat_balloon_->RemoveFromViewport();
}

void CharacterBase::OnSpeakEnd()
{
    chat_balloon_->RemoveFromViewport();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<CharacterBase>("CharacterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
