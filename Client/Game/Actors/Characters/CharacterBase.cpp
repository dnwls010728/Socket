#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");

    Sprite* sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Player.png");
    if (sprite)
    {
        renderer_->SetSprite(sprite, L"Player_0");
    }
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
