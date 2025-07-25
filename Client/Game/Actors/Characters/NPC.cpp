#include "pch.h"
#include "NPC.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "UI/UIInGameState.h"
#include "UI/Element/UINameTag.h"
#include "Windows/DX/Sprite.h"

NPC::NPC(const std::wstring& name) :
    CharacterBase(name),
    hide_duration_(5.f),
    show_duration_(5.f),
    is_showing_(false),
    timer_handle_(),
    sub_name_tag_(nullptr)
{
    SetLayer(ActorLayer::kNPC);
    
    collider_->SetOffset({ 0.f, .5f });
    collider_->SetSize({1.f, 1.f});
}

void NPC::BeginPlay()
{
    CharacterBase::BeginPlay();

    name_tag_->SetColor(Math::Color::Yellow);
    
    SetCharacterName(L"알렉스");

    if (auto state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        sub_name_tag_ = state->AddElement<UINameTag>(UINameTag::StaticClass(), L"SubNameTag");
        sub_name_tag_->SetText(L"방어구 상인");
        sub_name_tag_->SetColor(Math::Color::Yellow);
    }
    
    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Box.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Box_0");
    }

    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        Speak(L"어이쿠! 손이 미끄러졌네^^", show_duration_);
    }, hide_duration_, false);
}

void NPC::PhysicsTick(float delta_time)
{
    CharacterBase::PhysicsTick(delta_time);
       
    Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(GetTransform()->GetPosition());

    Math::Vector2 name_tag_offset = { -sub_name_tag_->GetSize().x * .5f, 28.f };
    sub_name_tag_->SetAbsolutePosition(screen_position + name_tag_offset);
}

void NPC::EndPlay(EndPlayReason type)
{
    CharacterBase::EndPlay(type);

    TimerManager::Get()->ClearTimer(timer_handle_);

    if (auto state = dynamic_cast<UIInGameState*>(UI::Get()->GetState()))
    {
        state->RemoveElement(sub_name_tag_);
    }
}

void NPC::OnSpeakEnd()
{
    CharacterBase::OnSpeakEnd();
    
    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        Speak(L"어이쿠! 손이 미끄러졌네^^", show_duration_);
    }, hide_duration_, false);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<NPC>("NPC")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
