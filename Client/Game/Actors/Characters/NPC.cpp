#include "pch.h"
#include "NPC.h"

#include "Actor/Component/BoxColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

NPC::NPC(const std::wstring& name) :
    CharacterBase(name),
    hide_duration_(5.f),
    show_duration_(5.f),
    is_showing_(false),
    timer_handle_()
{
    SetLayer(ActorLayer::kNPC);
    
    collider_->SetOffset({ 0.f, .5f });
    collider_->SetSize({1.f, 1.f});
}

void NPC::BeginPlay()
{
    CharacterBase::BeginPlay();

    SetCharacterName(L"도우미");
    
    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Box.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Box_0");
    }

    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        Speak(L"Hello, World!", show_duration_);
    }, hide_duration_, false);
}

void NPC::EndPlay(EndPlayReason type)
{
    CharacterBase::EndPlay(type);

    TimerManager::Get()->ClearTimer(timer_handle_);
}

void NPC::OnSpeakEnd()
{
    CharacterBase::OnSpeakEnd();
    
    TimerManager::Get()->SetTimer(timer_handle_, [&]()
    {
        Speak(L"Hello, World!", show_duration_);
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
