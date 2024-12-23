#include "pch.h"
#include "MobBase.h"

MobBase::MobBase(const std::wstring& kName) :
    CharacterBase(kName)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MobBase>("MobBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
