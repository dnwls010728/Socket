#include "pch.h"
#include "PropData.h"

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PropData>("PropData")
        .constructor<>()
        .property("id", &PropData::id)
        .property("name", &PropData::name)
        .property("sprite_path", &PropData::sprite_path)
        .property("animation_pack_path", &PropData::animation_pack_path)
        .property("weight", &PropData::weight);
}
