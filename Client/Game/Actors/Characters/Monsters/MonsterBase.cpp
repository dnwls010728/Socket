#include "pch.h"
#include "MonsterBase.h"

MonsterBase::MonsterBase(const std::wstring& kName) :
    CharacterBase(kName)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MonsterBase>("MonsterBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
