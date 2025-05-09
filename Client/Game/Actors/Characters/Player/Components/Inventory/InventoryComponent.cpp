#include "pch.h"
#include "InventoryComponent.h"

#include "Actor/Actor.h"

InventoryComponent::InventoryComponent(Actor* owner, const std::wstring& name) :
    ActorComponent(owner, name)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<InventoryComponent>("InventoryComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
