#pragma once
#include "ColliderComponent.h"

class CircleColliderComponent : public ColliderComponent
{
    SHADER_CLASS_HELPER(CircleColliderComponent)
    GENERATED_BODY(CircleColliderComponent, ColliderComponent)
    
public:
    CircleColliderComponent(Actor* owner, const std::wstring& kName);
    virtual ~CircleColliderComponent() override = default;

    void SetRadius(float radius);

private:
    virtual void SetShape() override;
    virtual void SetTriggerInternal() override;
    
    float radius_;
    
};
