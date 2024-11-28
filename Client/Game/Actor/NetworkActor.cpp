#include "pch.h"
#include "NetworkActor.h"

NetworkActor::NetworkActor(const std::wstring& kName) :
    Actor(kName),
    is_mine_(false)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<NetworkActor>("NetworkActor")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
