#pragma once
#include "ActorComponent.h"
#include "box2d/id.h"
#include "Math/Vector2.h"

struct Bounds;

struct PhysicsMaterial2D
{
    float friction = .3f;
    float bounciness = 0.f;
};

class ColliderComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(ColliderComponent)
    GENERATED_BODY(ColliderComponent, ActorComponent)
    
public:
    ColliderComponent(Actor* owner, const std::wstring& kName);
    virtual ~ColliderComponent() override = default;

    void SetOffset(const Math::Vector2& kOffset);
    void SetMaterial(const PhysicsMaterial2D& kMaterial);
    void SetTrigger(bool is_trigger);

    Math::Vector2 ClosestPoint(const Math::Vector2& kPoint) const;

    FORCEINLINE const Math::Vector2& GetOffset() const { return offset_; }

    const Bounds& GetBounds();

protected:
    friend class World;
    
    virtual void InitializeComponent() override;
    
    b2ShapeId shape_id_;

    Math::Vector2 offset_;

    PhysicsMaterial2D material_;

    bool is_trigger_;

private:
    virtual void SetShape();
    virtual void SetTriggerInternal();

    void SetMaterialIntermal();
    
};