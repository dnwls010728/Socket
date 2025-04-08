#include "pch.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const std::wstring& kName) :
    CharacterBase(kName)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<EnemyBase>("EnemyBase")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
