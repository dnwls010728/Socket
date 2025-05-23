#include "pch.h"
#include "ActorComponent.h"

#include "../Actor.h"

ActorComponent::ActorComponent(Actor* owner, const std::wstring& kName) :
    name_(kName),
    owner_(owner),
    has_begun_play_(false)
{
}

void ActorComponent::BeginPlay()
{
    has_begun_play_ = true;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ActorComponent>("ActorComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
