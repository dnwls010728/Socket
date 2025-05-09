#include "MapObject.h"

#include "Map.h"
#include "rttr/registration"

MapObject::MapObject(const std::wstring& kName):
    name_(kName),
    is_active_(true),
    map_(nullptr),
    unique_id_(0),
    location_(Math::Vector2::Zero()),
    velocity_(Math::Vector2::Zero())
{
    
}

void MapObject::SetActive(bool is_active)
{
    if (map_ != nullptr)
        map_->ActivateActor(this, is_active);
}

void MapObject::Destroy()
{
    if (map_ != nullptr)
        map_->DestroyActor(this);
}

void MapObject::Tick(float delta_time)
{
}

void MapObject::PhysicsTick(float delta_time)
{
}

void MapObject::PostTick(float delta_time)
{
}

void MapObject::OnEnable()
{
    is_active_ = true;
}

void MapObject::OnDisable()
{
    is_active_ = false;
}

void MapObject::BeginPlay()
{
}

void MapObject::EndPlay(EndPlayReason reason)
{
}

void MapObject::InitializeActor()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MapObject>("MapObject")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
