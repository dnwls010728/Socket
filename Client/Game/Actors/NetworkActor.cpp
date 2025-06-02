#include "pch.h"
#include "NetworkActor.h"

#include "GameInstance.h"
#include "Subsystems/SessionSubsystem.h"

NetworkActor::NetworkActor(const std::wstring& kName) :
    Actor(kName),
    object_id_(0),
    is_mine_(false)
{
}

void NetworkActor::SendPacket(Net::IPacket& packet)
{
    SessionSubsystem::Get()->SendPacket(packet);
}

void NetworkActor::ReceivePacket(Net::IPacket* packet)
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
