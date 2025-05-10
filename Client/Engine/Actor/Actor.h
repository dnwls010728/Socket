#pragma once
#include "ActorTag.h"
#include "box2d/id.h"
#include "Misc/DelegateMacros.h"
#include "Misc/EngineMacros.h"
#include "rttr/registration_friend.h"
#include "Time/TimerManager.h"

class ActorComponent;
class Actor;

enum class EndPlayReason : uint64_t;
class TransformComponent;

class Actor : public std::enable_shared_from_this<Actor>
{
    SHADER_CLASS_HELPER(Actor)
    GENERATED_BODY(Actor)
    
public:
    Actor(const std::wstring& kName);
    virtual ~Actor() = default;
    
    virtual float TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer);

    void SetActive(bool is_active);
    void Destroy();
    void SetLifeSpan(float life_span);
    void SetPersistent(bool is_persistent);
    
    bool CompareTag(ActorTag tag) const;

    template <std::derived_from<ActorComponent> T>
    std::shared_ptr<T> AddComponent(const std::wstring& kName);

    void GetComponents(const rttr::type& type, std::vector<ActorComponent*>& components);

    template <std::derived_from<ActorComponent> T>
    std::shared_ptr<T> GetComponent(const rttr::type& type);

    template <std::derived_from<Actor> T>
    T* SpawnActor(const rttr::type& kType, const std::wstring& kName);
    
    FORCEINLINE void SetTag(ActorTag tag) { tag_ = tag; }
    FORCEINLINE void SetLayer(ActorLayer layer) { layer_ = layer; }

    FORCEINLINE const std::wstring& GetName() const { return name_; }

    FORCEINLINE ActorTag GetTag() const { return tag_; }
    FORCEINLINE ActorLayer GetLayer() const { return layer_; }

    FORCEINLINE std::shared_ptr<TransformComponent> GetTransform() const { return transform_; }

    FORCEINLINE bool HasBegunPlay() const { return has_begun_play_; }
    FORCEINLINE bool IsActive() const { return is_active_; }
    FORCEINLINE bool IsPendingDeletion() const { return is_pending_destroy_; }

    FORCEINLINE void SetOwner(Actor* owner) { owner_ = owner; }
    FORCEINLINE Actor* GetOwner() const { return owner_; }

    FORCEINLINE void SetInstigator(Actor* instigator) { instigator_ = instigator; }
    FORCEINLINE Actor* GetInstigator() const { return instigator_; }

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

    FORCEINLINE virtual void PreInitializeComponents() {}
    FORCEINLINE virtual void PostInitializeComponents() {}
    
    virtual void BeginPlay();
    virtual void EndPlay(EndPlayReason type);

    virtual void PhysicsTick(float delta_time);
    virtual void Tick(float delta_time);
    virtual void PostTick(float delta_time);
    virtual void Render(float alpha);
    virtual void OnEnable();
    virtual void OnDisable();

    FORCEINLINE virtual void OnCollisionEnter(Actor* other) {}
    FORCEINLINE virtual void OnCollisionExit(Actor* other) {}
    FORCEINLINE virtual void OnTriggerEnter(Actor* other) {}
    FORCEINLINE virtual void OnTriggerExit(Actor* other) {}

    std::wstring name_;

    ActorTag tag_;
    ActorLayer layer_;

    b2BodyId body_id_;

    bool has_begun_play_;
    bool is_active_;
    bool is_pending_destroy_;

    std::vector<std::shared_ptr<ActorComponent>> components_;

    std::shared_ptr<TransformComponent> transform_;
    
    TimerHandle life_span_timer_;

    Actor* owner_;
    Actor* instigator_;

private:
    bool is_persistent_;

};

template <std::derived_from<ActorComponent> T>
std::shared_ptr<T> Actor::AddComponent(const std::wstring& kName)
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
    return std::static_pointer_cast<T>(components_.back());
}

template <std::derived_from<ActorComponent> T>
std::shared_ptr<T> Actor::GetComponent(const rttr::type& type)
{
    for (const auto& component : components_)
    {
        rttr::type component_type = rttr::type::get(*component);
        if (component_type.is_derived_from(type))
        {
            return std::static_pointer_cast<T>(component);
        }
    }

    return nullptr;
}

template <std::derived_from<Actor> T>
T* Actor::SpawnActor(const rttr::type& kType, const std::wstring& kName)
{
    return World::Get()->SpawnActor<T>(kType, kName);
}

FORCEINLINE bool IsValid(const Actor* actor)
{
    return actor && !actor->IsPendingDeletion();
}

FORCEINLINE bool IsValid(const std::shared_ptr<Actor>& actor)
{
    return actor && !actor->IsPendingDeletion();
}
