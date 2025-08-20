#include "pch.h"
#include "Portal.h"

Portal::Portal(const std::wstring& name) :
    Actor(name),
    id_(-1)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Portal>("Portal")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
