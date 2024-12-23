#include "pch.h"
#include "WeaponData.h"

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<WeaponData>("WeaponData")
        .constructor<>()
        (
            policy::ctor::as_object
        )
        .property("id", &WeaponData::id)
        .property("name", &WeaponData::name)
        .property("description", &WeaponData::description)
        .property("size", &WeaponData::size)
        .property("capacity", &WeaponData::capacity)
        .property("reload_time", &WeaponData::reload_time)
        .property("speed", &WeaponData::speed)
        .property("power", &WeaponData::power)
        .property("delay", &WeaponData::delay)
        .property("range", &WeaponData::range)
        .property("spread", &WeaponData::spread);
}
