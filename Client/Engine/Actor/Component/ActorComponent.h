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
    inline virtual void OnEnable() {}
    inline virtual void OnDisable() {}
    
    Actor* owner_;
    std::wstring name_;

private:
    friend class Actor;
    friend class PlayerController;

    bool has_begun_play_;
    
};
