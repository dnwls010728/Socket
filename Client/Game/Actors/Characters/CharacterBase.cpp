#include "pch.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(const std::wstring& kName) :
    Actor(kName)
{
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
