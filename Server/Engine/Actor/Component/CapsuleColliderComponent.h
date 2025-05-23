#pragma once
#include "ColliderComponent.h"

enum class CapsuleDirection
{
    Horizontal,
    Vertical
};

class CapsuleColliderComponent : public ColliderComponent
{
    SHADER_CLASS_HELPER(CapsuleColliderComponent)
    GENERATED_BODY(CapsuleColliderComponent, ColliderComponent)
    
public:
    CapsuleColliderComponent(Actor* owner, const std::wstring& kName);
    virtual ~CapsuleColliderComponent() override = default;

    void SetSize(Math::Vector2 size);
    void SetDirection(CapsuleDirection direction);

private:
    virtual void SetShape() override;
    virtual void SetTriggerInternal() override;

    CapsuleDirection direction_;
    Math::Vector2 size_;
    
};
