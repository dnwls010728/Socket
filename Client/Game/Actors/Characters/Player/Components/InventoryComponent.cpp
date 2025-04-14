#include "pch.h"
#include "InventoryComponent.h"

InventoryComponent::InventoryComponent(Actor* owner, const std::wstring& name)
    : ActorComponent(owner, name),mCurrentWeight(0.f),mMaxWeight(100.f)
{
}

bool InventoryComponent::AddItem(const PropData& propData)
{
    const float itemWeight = propData.weight;
    if (mCurrentWeight + itemWeight > mMaxWeight)
        return false;

    mCurrentWeight += itemWeight;
    return true;
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
