#include "pch.h"
#include "PlayerCharacter.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

PlayerCharacter::PlayerCharacter(const std::wstring& kName) :
    CharacterBase(kName)
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

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerCharacter>("PlayerCharacter")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
