#pragma once
#include "Actor/Actor.h"

class PooledObject;

DECLARE_DELEGATE(PooledObjectDespawnDelegate, PooledObject*);

class PooledObject : public Actor
{
    SHADER_CLASS_HELPER(PooledObject)
    GENERATED_BODY(PooledObject, Actor)
    
public:
    PooledObject(const std::wstring& kName);
    virtual ~PooledObject() override = default;

    void Deactivate();

    PooledObjectDespawnDelegate DespawnHandler;

    FORCEINLINE void SetPoolIndex(int index) { pool_index_ = index; }
    FORCEINLINE int GetPoolIndex() const { return pool_index_; }

protected:
    int pool_index_;
    
};
