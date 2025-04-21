#include "pch.h"
#include "NetworkSubsystem.h"

#include <CustomPacket.h>

#include "GameInstance.h"
#include "SessionSubsystem.h"
#include "Actors/Characters/Player/PlayerCharacter.h"
#include "Level/CameraManager.h"

NetworkSubsystem::NetworkSubsystem()
{
}

void NetworkSubsystem::Init()
{
    WorldSubsystem::Init();

    GET_SESSION()->packet_handler.Add(this, &NetworkSubsystem::ProcessPackets);
    
}

void NetworkSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    GET_SESSION()->packet_handler.Remove(this, &NetworkSubsystem::ProcessPackets);
}

void NetworkSubsystem::OnWorldBeginPlay()
{
    WorldSubsystem::OnWorldBeginPlay();

    if (GET_SESSION()->IsInGame())
    {
        std::shared_ptr<PlayerCharacter> player_character = World::Get()->SpawnActor<PlayerCharacter>(PlayerCharacter::StaticClass(), L"PlayerCharacter");
        if (IsValid(player_character))
        {
            CameraManager::Get()->SetTarget(player_character);
        }
    }
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
    switch (packet->GetPacketID())
    {
    case SpawnPlayerPacket::StaticPacketID:
        {
            SpawnPlayerPacket* spawn_player_packet = static_cast<SpawnPlayerPacket*>(packet.get());
            Logger::Print(L"플레이어 생성");
        }
        break;

    case DestroyPlayerPacket::StaticPacketID:
        {
            DestroyPlayerPacket* destroy_player_packet = static_cast<DestroyPlayerPacket*>(packet.get());
            Logger::Print(L"플레이어 파괴");
        }
        break;
        
    default:
        break;
    }
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
