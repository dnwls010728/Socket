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

    void SetMaxWeight(float maxWeight);
    FORCEINLINE bool AddItem(const PropData& propData);
    FORCEINLINE float GetCurrentWeight() const{return mCurrentWeight;}
    FORCEINLINE float GetMaxWeight(){return mMaxWeight;}


private:
    float mCurrentWeight;
    float mMaxWeight;
};
