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

    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem) session_subsystem->packet_handler.Add(this, &NetworkSubsystem::ProcessPackets);
    
}

void NetworkSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    SessionSubsystem* session_subsystem = GameInstance::Get()->GetSubsystem<SessionSubsystem>();
    if (session_subsystem) session_subsystem->packet_handler.Remove(this, &NetworkSubsystem::ProcessPackets);
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

void NetworkSubsystem::ProcessPackets(const std::shared_ptr<Net::IPacket>& packet)
{
    Logger::Print(L"Packet ID: %d", packet->GetPacketID());
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
