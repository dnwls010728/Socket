#include "pch.h"
#include "NetworkSubsystem.h"

#include "GameInstance.h"
#include "SessionSubsystem.h"

NetworkSubsystem::NetworkSubsystem()
{
}

void NetworkSubsystem::Init()
{
    WorldSubsystem::Init();

    Logger::Print(L"TestWorldSubsystem initialized.");

    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem) session_subsystem->OnPacketReceived(this, &NetworkSubsystem::ProcessPackets);
    
}

void NetworkSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    Logger::Print(L"TestWorldSubsystem deinitialized.");
}

void NetworkSubsystem::Tick(float delta_time)
{
    Tickable::Tick(delta_time);

    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem)
    {
        session_subsystem->ProcessPackets();
    }
}

bool NetworkSubsystem::ProcessPackets(std::shared_ptr<Net::IPacket> packet)
{
    Logger::Print(L"Packet ID: %d", packet->GetPacketID());
    return false;
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<NetworkSubsystem>("NetworkSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
