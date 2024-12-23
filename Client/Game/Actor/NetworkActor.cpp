#include "pch.h"
#include "NetworkActor.h"

NetworkActor::NetworkActor(const std::wstring& kName) :
    Actor(kName),
    is_mine_(false),
    packet_id_(-1),
    received_position_(Math::Vector2::Zero())
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
