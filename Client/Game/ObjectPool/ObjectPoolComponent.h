#pragma once
#include "PooledObject.h"
#include "Actor/Component/ActorComponent.h"

class ObjectPoolComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(ObjectPoolComponent)
    GENERATED_BODY(ObjectPoolComponent, ActorComponent)
    
public:
    ObjectPoolComponent(Actor* owner, const std::wstring& kName);
    virtual ~ObjectPoolComponent() override = default;

    virtual void BeginPlay() override;

    PooledObject* SpawnPooledObject();

    void OnPooledObjectDespawn(PooledObject* pool_actor);

    FORCEINLINE void SetPooledObjectClass(const rttr::type& type) { pooled_object_class_ = type; }

protected:
    rttr::type pooled_object_class_;
    
    std::vector<PooledObject*> object_pool_;
    std::vector<int> spawned_pool_indexes_;
    
};
