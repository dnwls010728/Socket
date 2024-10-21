#include "pch.h"
#include "Object.h"

Type::uint64 Object::next_instance_id_ = 0;

Object::Object() :
    instance_id_(next_instance_id_++)
{
}

bool Object::operator==(const Object& kOther) const
{
    return instance_id_ == kOther.instance_id_;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Object>("Object")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
