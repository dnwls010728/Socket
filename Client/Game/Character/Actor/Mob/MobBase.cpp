#include "pch.h"
#include "MobBase.h"

#include "Character/Component/FSM/StateMachine.h"

MobBase::MobBase(const std::wstring& kName) :
    CharacterBase(kName)
{
    SetLayer(ActorLayer::kMob);

    state_machine_ = AddComponent<StateMachine>(L"StateMachine");
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
