#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Camera.h"
#include "Actor/Component/CapsuleColliderComponent.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Input/Keyboard.h"
#include "Resource/ResourceManager.h"
#include "UI/Canvas.h"
#include "UI/Widget/Text.h"
#include "Windows/DX/Sprite.h"

Type::uint32 current_player_id = 0;

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    nickname_(L"")
{
    // sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Default\\Capsule.png");
    // sprite_->Split(1, 1, Sprite::kBottom);
    
    sprite_ = ResourceManager::Get()->Load<Sprite>(L"Sprites\\Character\\PlayerSheet.png");

    renderer_->SetSprite(sprite_);
    // renderer_->SetColor(Math::Color::Black);

    collider_->SetOffset({0.f, 1.f});

#pragma region Nickname
    Canvas* canvas = Canvas::Get();
    
    nickname_text_ = canvas->AddWidget<Text>(L"Player Nickname");
    nickname_text_->AttachToWidget(canvas->GetRootWidget());
    nickname_text_->SetSize({200.f, 100.f});
    nickname_text_->SetAlignment(Text::kMiddleCenter);
#pragma endregion
    
}

void PlayerCharacter::SetNickname(const std::wstring& kNickname)
{
    nickname_ = kNickname;
    nickname_text_->SetText(nickname_);
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Camera::Get()->SetTarget(this);
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);
    Keyboard* keyboard = Keyboard::Get();
    const int h = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
    velocity_.x = h * 5.f;
}

void PlayerCharacter::PostTick(float delta_time)
{
    CharacterBase::PostTick(delta_time);

    TransformComponent* transform = GetTransform();
    
    Math::Vector2 position = transform->GetPosition();
    Math::Vector2 screen_position = Renderer::Get()->ConvertWorldToScreen(position);

    nickname_text_->SetPosition(screen_position);
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
