#pragma once
#include "Object.h"
#include "box2d/id.h"
#include "Misc/DelegateMacros.h"
#include "Misc/EngineMacros.h"
#include "Time/TimerManager.h"

class ActorComponent;
class Actor;

DECLARE_DELEGATE(ContactSignature, Actor*);

enum class EndPlayReason : MathTypes::uint64;
class TransformComponent;

class Actor : public Object
{
    SHADER_CLASS_HELPER(Actor)
    GENERATED_BODY(Actor, Object)
    
public:
    Actor(const std::wstring& kName);
    virtual ~Actor() override = default;

    void SetActive(bool is_active);
    void Destroy();
    void SetLifeSpan(float life_span);
    
    bool CompareTag(ActorTag tag) const;

    template <std::derived_from<ActorComponent> T>
    T* AddComponent(const std::wstring& kName);

    ActorComponent* GetComponent(const rttr::type& type);

    template <std::derived_from<Actor> T>
    T* SpawnActor(const std::wstring& kName);
    
    inline void SetTag(ActorTag tag) { tag_ = tag; }
    inline void SetLayer(ActorLayer layer) { layer_ = layer; }

    inline const std::wstring& GetName() const { return name_; }

    inline ActorTag GetTag() const { return tag_; }
    inline ActorLayer GetLayer() const { return layer_; }

    inline TransformComponent* GetTransform() const { return transform_.get(); }

    inline bool IsActive() const { return is_active_; }
    inline bool IsPendingDeletion() const { return is_pending_destroy_; }
    
    ContactSignature on_collision_enter;
    ContactSignature on_collision_exit;

    ContactSignature on_trigger_enter;
    ContactSignature on_trigger_exit;

protected:
    friend class World;
    friend class Level;
    friend class TransformComponent;
    friend class ColliderComponent;
    friend class BoxColliderComponent;
    friend class CircleColliderComponent;
    friend class CapsuleColliderComponent;
    friend class RigidBody2DComponent;
    friend class Physics2D;
    
    void InitializeActor();
    void InitializeComponents();
    void UninitializeComponents();
    void CreateBody();
    void OnLifeSpanExpired();

    inline virtual void PreInitializeComponents() {}
    inline virtual void PostInitializeComponents() {}
    
    virtual void BeginPlay();
    virtual void EndPlay(EndPlayReason type);
    virtual void Destroyed();

    virtual void PhysicsTick(float delta_time);
    virtual void Tick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void Render(float alpha);
    virtual void OnEnable();
    virtual void OnDisable();

    virtual void OnCollisionEnter(Actor* other);
    virtual void OnCollisionExit(Actor* other);
    virtual void OnTriggerEnter(Actor* other);
    virtual void OnTriggerExit(Actor* other);

    std::wstring name_;

    ActorTag tag_;
    ActorLayer layer_;

    b2BodyId body_id_;

    bool is_active_;
    bool is_pending_destroy_;

    std::vector<std::shared_ptr<ActorComponent>> components_;

    std::shared_ptr<TransformComponent> transform_;
    
    TimerHandle life_span_timer_;

};

template <std::derived_from<ActorComponent> T>
T* Actor::AddComponent(const std::wstring& kName)
{
    // rttr::type type = rttr::type::get<T>();
    // for (const auto& kComponent : components_)
    // {
    //     rttr::type component_type = rttr::type::get(*kComponent);
    //     if (component_type == type)
    //     {
    //         return nullptr;
    //     }
    // }
    
    components_.push_back(std::make_shared<T>(this, kName));
    return static_cast<T*>(components_.back().get());
}

template <std::derived_from<Actor> T>
T* Actor::SpawnActor(const std::wstring& kName)
{
    return World::Get()->SpawnActor<T>(kName);
}

inline bool IsValid(Actor* actor)
{
    return actor && !actor->IsPendingDeletion();
}
