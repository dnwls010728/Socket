#include "pch.h"
#include "CharacterBase.h"

#include "DebugDrawHelper.h"
#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/RigidBody2DComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Actors/Components/StateMachineComponent.h"
#include "Components/Controller2DComponent.h"
#include "UI/ChatBalloon.h"
#include "UI/NameTag.h"
#include "UI/UIManager.h"
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
    collider_->SetSize({1.f, 1.f});

    controller_ = AddComponent<Controller2DComponent>(L"Controller2D");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetZOrder(std::numeric_limits<int32_t>::max());

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

    timer_manager->SetTimer(chat_balloon_timer_handle_, this, &CharacterBase::OnSpeakEnd, duration, false);
}

void CharacterBase::BeginPlay()
{
    NetworkActor::BeginPlay();

    name_tag_ = UI::NameTag::Create(L"NameTag");
    name_tag_->SetText(character_name_);
    
    chat_balloon_ = UI::ChatBalloon::Create(L"ChatBalloon");
    chat_balloon_->SetSize({8.f, 8.f});

    UI::Manager* ui_manager = UI::Manager::Get();
    ui_manager->AddToViewport(name_tag_);
}

void CharacterBase::PhysicsTick(float delta_time)
{
    NetworkActor::PhysicsTick(delta_time);
    
    UI::Manager* ui_manager = UI::Manager::Get();
    Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(GetTransform()->GetPosition());

    if (ui_manager->IsInViewport(name_tag_))
        name_tag_->SetPosition(screen_position + Math::Vector2::Up() * 50.f);
    
    if (ui_manager->IsInViewport(chat_balloon_))
        chat_balloon_->SetPosition(screen_position + Math::Vector2::Down() * 40.f);
}

void CharacterBase::EndPlay(EndPlayReason type)
{
    NetworkActor::EndPlay(type);
    
    TimerManager::Get()->ClearTimer(chat_balloon_timer_handle_);

    UI::Manager* ui_manager = UI::Manager::Get();
    
    ui_manager->RemoveFromViewport(chat_balloon_);
    ui_manager->RemoveFromViewport(name_tag_);
}

void CharacterBase::OnSpeakEnd()
{
    UI::Manager::Get()->RemoveFromViewport(chat_balloon_);
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
