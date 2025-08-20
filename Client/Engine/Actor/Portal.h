#pragma once

class AnimatorComponent;

class Portal : public Actor
{
    SHADER_CLASS_HELPER(Portal)
    GENERATED_BODY(Portal, Actor)
    
public:
    Portal(const std::wstring& name);
    virtual ~Portal() override = default;

    FORCEINLINE void SetID(int32_t id) { id_ = id; }
    FORCEINLINE int32_t GetID() const { return id_; }

protected:
    virtual void BeginPlay() override;

private:
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<AnimatorComponent> animator_;
    std::shared_ptr<BoxColliderComponent> collider_;
    
    int32_t id_;
    
};
