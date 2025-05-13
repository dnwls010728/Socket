#include "pch.h"
#include "MonsterBase.h"

MonsterBase::MonsterBase(std::wstring kName) : CharacterBase(kName)
{
}

void MonsterBase::Tick(float delta_time)
{
    Actor::Tick(delta_time);
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