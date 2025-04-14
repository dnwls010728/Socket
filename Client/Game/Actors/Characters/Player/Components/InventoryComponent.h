#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Data/PropData.h"
#include <string>


class InventoryComponent:public ActorComponent
{
    SHADER_CLASS_HELPER(InventoryComponent)
    GENERATED_BODY(InventoryComponent,ActorComponent)
    
public:
    InventoryComponent(Actor* owner,const std::wstring&kName);
    virtual ~InventoryComponent() override=default;

    FORCEINLINE void SetMaxWeight(float maxWeight)
    {
        mMaxWeight = maxWeight;
    }
    bool AddItem(const PropData& propData);
    FORCEINLINE float GetCurrentWeight() const{return mCurrentWeight;}
    FORCEINLINE float GetMaxWeight() const{return mMaxWeight;}


private:
    float mCurrentWeight;
    float mMaxWeight;
};
