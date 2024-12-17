#include "pch.h"
#include "AnimatorComponent.h"

#include "Actor/Actor.h"

AnimatorComponent::AnimatorComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    animation_pack_(nullptr)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<AnimatorComponent>("AnimatorComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
