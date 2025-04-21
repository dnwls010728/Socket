#include "pch.h"
#include "Map_Begin.h"

Map_Begin::Map_Begin(const std::wstring& kName) :
    Level(kName)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Map_Begin>("Map_Begin")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
