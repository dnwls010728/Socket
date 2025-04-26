#include "pch.h"
#include "Actor.h"

#include "Enums.h"
#include "box2d/box2d.h"
#include "Component/TransformComponent.h"
#include "Level/World.h"

Actor::Actor(const std::wstring& kName) :
    name_(kName),
    tag_(ActorTag::kNone),
    layer_(ActorLayer::kDefault),
    is_active_(true),
    is_pending_destroy_(false),
    is_persistent_(false),
    components_()
{
    transform_ = AddComponent<TransformComponent>(L"Transform");
    CHECK(transform_);
}

void Actor::BeginPlay()
{
    if (b2Body_IsValid(body_id_) && !b2Body_IsEnabled(body_id_)) b2Body_Enable(body_id_);
    
    for (const auto& kComponent : components_)
    {
        kComponent->BeginPlay();
    }
}

void Actor::EndPlay(EndPlayReason type)
{
    for (const auto& kComponent : components_)
    {
        kComponent->EndPlay(type);
    }
    
    UninitializeComponents();

    components_.clear();

    if (b2Body_IsValid(body_id_))
    {
        b2DestroyBody(body_id_);
        body_id_ = b2_nullBodyId;
    }
}

void Actor::PhysicsTick(float delta_time)
{
    for (const auto& kComponent : components_)
    {
        kComponent->PhysicsTickComponent(delta_time);
    }
}

void Actor::Tick(float delta_time)
{
    for (const auto& kComponent : components_)
    {
        kComponent->TickComponent(delta_time);
    }
}

void Actor::PostTick(float delta_time)
{
    for (const auto& kComponent : components_)
    {
        kComponent->PostTickComponent(delta_time);
    }
}

void Actor::Render(float alpha)
{
    for (const auto& component : components_)
    {
        component->Render(alpha);
    }
}

void Actor::OnEnable()
{
    is_active_ = true;
    if (b2Body_IsValid(body_id_)) b2Body_Enable(body_id_);

    for (const auto& component : components_)
    {
        component->OnEnable();
    }
}

void Actor::OnDisable()
{
    is_active_ = false;
    if (b2Body_IsValid(body_id_)) b2Body_Disable(body_id_);

    for (const auto& component : components_)
    {
        component->OnDisable();
    }
}

float Actor::TakeDamage(float damage_amount, Actor* event_instigator, Actor* damage_causer)
{
    return damage_amount;
}

void Actor::SetActive(bool is_active)
{
    World::Get()->ActivateActor(this, is_active);
}

void Actor::Destroy()
{
    World::Get()->DestroyActor(this);
}

void Actor::SetLifeSpan(float life_span)
{
    if (life_span > 0.f)
    {
         TimerManager::Get()->SetTimer(life_span_timer_, this, &Actor::OnLifeSpanExpired, life_span);
    }
    else
    {
        TimerManager::Get()->ClearTimer(life_span_timer_);
    }
}

void Actor::SetPersistent(bool is_persistent)
{
    is_persistent_ = is_persistent;
}

bool Actor::CompareTag(ActorTag tag) const
{
    return tag_ == tag;
}

void Actor::GetComponents(const rttr::type& type, std::vector<ActorComponent*>& components)
{
    for (const auto& component : components_)
    {
        rttr::type component_type = rttr::type::get(*component);
        if (component_type.is_derived_from(type))
        {
            components.push_back(component.get());
        }
    }
}

void Actor::InitializeActor()
{
    PreInitializeComponents();
    InitializeComponents();
    PostInitializeComponents();
}

void Actor::InitializeComponents()
{
    for (const auto& component : components_)
    {
        component->InitializeComponent();
    }
}

void Actor::UninitializeComponents()
{
    for (const auto& component : components_)
    {
        component->UninitializeComponent();
    }
}

void Actor::CreateBody()
{
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.userData = this;

    body_id_ = b2CreateBody(World::Get()->world_id_, &body_def);
    b2Body_Disable(body_id_);
}

void Actor::OnLifeSpanExpired()
{
    Destroy();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Actor>("Actor")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
