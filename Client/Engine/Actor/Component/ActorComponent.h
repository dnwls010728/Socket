#pragma once
#include <memory>

#include "Object.h"
#include "Misc/EngineMacros.h"

enum class EndPlayReason : Type::uint64;
class ActorComponent : public Object
{
    SHADER_CLASS_HELPER(ActorComponent)
    GENERATED_BODY(ActorComponent, Object)
    
public:
    ActorComponent(class Actor* owner, const std::wstring& kName);
    virtual ~ActorComponent() override = default;

    inline Actor* GetOwner() const { return owner_; }
    inline std::wstring GetName() const { return name_; }

    inline bool HasBegunPlay() const { return has_begun_play_; }

protected:
    virtual void BeginPlay();
    
    inline virtual void InitializeComponent() {}
    inline virtual void UninitializeComponent() {}
    inline virtual void EndPlay(EndPlayReason type) {}
    inline virtual void PhysicsTickComponent(float delta_time) {}
    inline virtual void TickComponent(float delta_time) {}
    inline virtual void PostTickComponent(float delta_time) {}
    inline virtual void Render(float alpha) {}
    
    Actor* owner_;
    std::wstring name_;

private:
    friend class Actor;
    friend class PlayerController;

    bool has_begun_play_;
    
};
