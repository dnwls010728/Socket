#include "Actor.h"

#include "Map/Map.h"
#include "box2d/box2d.h"
#include "box2d/types.h"

Actor::Actor(const std::wstring& kName):
    name_(kName),
    is_active_(true),
    map_(nullptr),
    unique_id_(0),
    location_(Math::Vector2::Zero()),
    velocity_(Math::Vector2::Zero())
{
    
}

void Actor::CreateBody()
{
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.userData = this;

    //body_id_ = b2CreateBody(World::Get()->world_id_, &body_def);
    b2Body_Disable(body_id_);
}

void Actor::SetActive(bool is_active)
{
    if (map_ != nullptr)
        map_->ActivateActor(this, is_active);
}

void Actor::Destroy()
{
    if (map_ != nullptr)
        map_->DestroyActor(this);
}

void Actor::Tick(float delta_time)
{
}

void Actor::PhysicsTick(float delta_time)
{
}

void Actor::PostTick(float delta_time)
{
}

void Actor::OnEnable()
{
    is_active_ = true;
}

void Actor::OnDisable()
{
    is_active_ = false;
}

void Actor::BeginPlay()
{
}

void Actor::EndPlay(EndPlayReason reason)
{
}

void Actor::InitializeActor()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Actor>("MapObject")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
