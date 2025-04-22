#include "pch.h"
#include "NetworkActor.h"

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"

NetworkActor::NetworkActor(const std::wstring& kName) :
    Actor(kName),
    unique_id_(0),
    is_owner_(false)
{
}

void NetworkActor::SendPacket(Net::IPacket& packet)
{
    GET_SESSION()->SendPacket(packet);
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
