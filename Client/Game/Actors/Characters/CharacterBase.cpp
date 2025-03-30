#include "pch.h"
#include "CharacterBase.h"

#include "Actor/Component/SpriteRendererComponent.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
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
