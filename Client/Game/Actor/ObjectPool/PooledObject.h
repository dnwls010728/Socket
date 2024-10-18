#pragma once
#include "Actor/Actor.h"

class PooledObject;

DECLARE_DELEGATE(OnPooledObjectDespawn, PooledObject*);

class PooledObject : public Actor
{
    SHADER_CLASS_HELPER(PooledObject)
    GENERATED_BODY(PooledObject, Actor)
    
public:
    PooledObject(const std::wstring& kName);
    virtual ~PooledObject() override = default;

    void Deactivate();

    OnPooledObjectDespawn on_despawn;

    inline void SetPoolIndex(int index) { pool_index_ = index; }
    inline int GetPoolIndex() const { return pool_index_; }

protected:
    virtual void OnEnable() override;
    
    int pool_index_;
    
};
