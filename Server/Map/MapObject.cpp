#include "MapObject.h"
#include "rttr/registration"

MapObject::MapObject(const std::wstring& name)
{
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

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<MapObject>("MapObject")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
