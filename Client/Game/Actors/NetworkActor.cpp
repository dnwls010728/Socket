#include "pch.h"
#include "NetworkActor.h"

NetworkActor::NetworkActor(const std::wstring& kName) :
    Actor(kName),
    id_(0)
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
