#pragma once
#include <memory>

#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"

enum class EndPlayReason : Type::uint64;
class ActorComponent : public std::enable_shared_from_this<ActorComponent>
{
    SHADER_CLASS_HELPER(ActorComponent)
    GENERATED_BODY(ActorComponent)
    
public:
    ActorComponent(class Actor* owner, const std::wstring& kName);
    virtual ~ActorComponent() = default;

    FORCEINLINE Actor* GetOwner() const { return owner_; }
    FORCEINLINE std::wstring GetName() const { return name_; }

    FORCEINLINE bool HasBegunPlay() const { return has_begun_play_; }

protected:
    virtual void BeginPlay();
    
    FORCEINLINE virtual void InitializeComponent() {}
    FORCEINLINE virtual void UninitializeComponent() {}
    FORCEINLINE virtual void EndPlay(EndPlayReason type) {}
    FORCEINLINE virtual void PhysicsTickComponent(float delta_time) {}
    FORCEINLINE virtual void TickComponent(float delta_time) {}
    FORCEINLINE virtual void PostTickComponent(float delta_time) {}
    FORCEINLINE virtual void Render(float alpha) {}
    FORCEINLINE virtual void OnEnable() {}
    FORCEINLINE virtual void OnDisable() {}
    
    Actor* owner_;
    std::wstring name_;

private:
    friend class Actor;

    bool has_begun_play_;
    
};
