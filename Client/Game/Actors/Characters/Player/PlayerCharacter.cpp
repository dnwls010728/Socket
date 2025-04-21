#include "pch.h"
#include "PlayerCharacter.h"

#include <CustomPacket.h>

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Input/Keyboard.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName),
    is_owner_(false)
{
}

void PlayerCharacter::BeginPlay()
{
    CharacterBase::BeginPlay();

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Default\\Circle.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Circle_0");
    }
}

void PlayerCharacter::Tick(float delta_time)
{
    CharacterBase::Tick(delta_time);

    std::shared_ptr<TransformComponent> transform = GetTransform();
    Math::Vector2 position = transform->GetPosition();

    if (IsOwner())
    {
        Keyboard* keyboard = Keyboard::Get();
        int h = keyboard->GetKey(VK_RIGHT) - keyboard->GetKey(VK_LEFT);
        int v = keyboard->GetKey(VK_UP) - keyboard->GetKey(VK_DOWN);

        Math::Vector2 direction = {static_cast<float>(h), static_cast<float>(v)};

        transform->Translate(direction * 5.f * delta_time);

        MovePlayerPacket packet;
        packet.x = position.x;
        packet.y = position.y;
        SendPacket(packet);
    }
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
