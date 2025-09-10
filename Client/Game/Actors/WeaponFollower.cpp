#include "pch.h"
#include "WeaponFollower.h"

WeaponFollower::WeaponFollower(const std::wstring& name) :
    Actor(name)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<WeaponFollower>("WeaponFollower")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
