#include "pch.h"
#include "SessionSubsystem.h"

#include <CustomPacket.h>
#include <CustomSerializer.h>

#include "GameInstance.h"
#include "NetworkManager.h"
#include "PacketHandlers/AddItemHandler.h"
#include "PacketHandlers/ChangeItemCountHandler.h"
#include "PacketHandlers/ChatMessageHandler.h"
#include "PacketHandlers/CheckNameHandler.h"
#include "PacketHandlers/CreateCharacterHandler.h"
#include "PacketHandlers/DeleteCharacterHandler.h"
#include "PacketHandlers/ObjectDestroyHandler.h"
#include "PacketHandlers/DropItemHandler.h"
#include "PacketHandlers/LoginHandler.h"
#include "PacketHandlers/MapLoadHandler.h"
#include "PacketHandlers/MoveItemHandler.h"
#include "PacketHandlers/MovePlayerHandler.h"
#include "PacketHandlers/PlayerAnimationHandler.h"
#include "PacketHandlers/ObjectPositionHandler.h"
#include "PacketHandlers/ObjectAnimationHandler.h"
#include "PacketHandlers/PlayerStatsUpdateHandler.h"
#include "PacketHandlers/SelectCharacterHandler.h"
#include "PacketHandlers/ObjectSpawnHandler.h"
#include "PacketHandlers/TakeDamageHandler.h"
#include "UI/UILoginState.h"
#include "Windows/WindowsApplication.h"

SessionSubsystem::SessionSubsystem() :
    state_(SessionState::kNone),
    handlers_()
{
}

void SessionSubsystem::Init()
{
    GameInstanceSubsystem::Init();
    
    // bool result = Connect({"175.198.74.36", 9000});
    // bool result = Connect({"222.108.73.155", 9000});
#ifdef _DEBUG
    bool result = Connect({"127.0.0.1", 9101});
#else
    bool result = Connect({"58.79.118.105", 9101});
#endif
    if (!result)
    {
        MessageBox(nullptr, L"서버와 연결할 수 없습니다.", EngineSettings::Get()->GetWindowTitle().c_str(), MB_OK);
        WindowsApplication::Get()->QuitApplication();
        return;
    }

    client_socket_.SetDisconnectedCallback([]()
    {
        MessageBox(nullptr, L"서버와의 연결이 끊어졌습니다.", EngineSettings::Get()->GetWindowTitle().c_str(), MB_OK);
        WindowsApplication::Get()->QuitApplication();
    });

#pragma region 핸들러 등록
    handlers_.emplace(
        LoginResponse::StaticPacketID,
        std::make_unique<LoginHandler>()
    );

    handlers_.emplace(
        CheckNameResponse::StaticPacketID,
        std::make_unique<CheckNameHandler>()
    );

    handlers_.emplace(
        CreateCharacterResponse::StaticPacketID,
        std::make_unique<CreateCharacterHandler>()
    );

    handlers_.emplace(
        DeleteCharacterResponse::StaticPacketID,
        std::make_unique<DeleteCharacterHandler>()
    );
    
    handlers_.emplace(
        SelectCharacterResponse::StaticPacketID,
        std::make_unique<SelectCharacterHandler>()
    );

    handlers_.emplace(
        MapLoadPacket::StaticPacketID,
        std::make_unique<MapLoadHandler>()
    );

    handlers_.emplace(
        AddItemPacket::StaticPacketID,
        std::make_unique<AddItemHandler>()
    );

    handlers_.emplace(
        ChangeItemCountPacket::StaticPacketID,
        std::make_unique<ChangeItemCountHandler>()
    );

    handlers_.emplace(
        MoveItemResponse::StaticPacketID,
        std::make_unique<MoveItemHandler>()
    );

    handlers_.emplace(
        MovePlayerPacket::StaticPacketID,
        std::make_unique<MovePlayerHandler>()
    );

    handlers_.emplace(
        PlayerAnimationPacket::StaticPacketID,
        std::make_unique<PlayerAnimationHandler>()
    );

    handlers_.emplace(
        ChatMessagePacket::StaticPacketID,
        std::make_unique<ChatMessageHandler>()
    );

    handlers_.emplace(
        ObjectSpawnPacket::StaticPacketID,
        std::make_unique<ObjectSpawnHandler>()
    );

    handlers_.emplace(
        ObjectDestroyPacket::StaticPacketID,
        std::make_unique<ObjectDestroyHandler>()
    );

    handlers_.emplace(
        ObjectPositionPacket::StaticPacketID,
        std::make_unique<ObjectPositionHandler>()
    );

    handlers_.emplace(
        ObjectAnimationPacket::StaticPacketID,
        std::make_unique<ObjectAnimationHandler>()
    );

    handlers_.emplace(
        PlayerStatsUpdatePacket::StaticPacketID,
        std::make_unique<PlayerStatsUpdateHandler>()
    );

    handlers_.emplace(
        TakeDamagePacket::StaticPacketID,
        std::make_unique<TakeDamageHandler>()
    );

    handlers_.emplace(
        DropItemResponse::StaticPacketID,
        std::make_unique<DropItemHandler>()
    );
#pragma endregion

    SetState(SessionState::kConnected);
}

void SessionSubsystem::Deinit()
{
    GameInstanceSubsystem::Deinit();

    Disconnect();
}

void SessionSubsystem::ProcessPackets()
{
    client_socket_.ProcessPacketsFromQueue([&](Net::TCP::ReceivedPacketInfo& received_packet)
    {
        std::shared_ptr<Net::IPacket> packet = std::move(received_packet.packet);
        packet_handler.Execute(packet);

        auto it = handlers_.find(packet->GetPacketID());
        if (it != handlers_.end())
        {
            // 패킷 핸들링 실패 시 애플리케이션 종료
            if (!it->second->Handle(packet.get()))
            {
                uint16_t packet_id = packet->GetPacketID(); // 디버깅용
                MessageBox(nullptr, (L"패킷 핸들링 실패: " + std::to_wstring(packet_id)).c_str(), EngineSettings::Get()->GetWindowTitle().c_str(), MB_OK);
                WindowsApplication::Get()->QuitApplication();
            }
        }
    });
}

void SessionSubsystem::SendPacket(Net::IPacket& packet)
{
    client_socket_.SendPacket(packet);
}

SessionSubsystem* SessionSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<SessionSubsystem>();
}

bool SessionSubsystem::Connect(const Net::NetAddress& address)
{
    Net::WSAInit();
    return client_socket_.Start(address);
}

void SessionSubsystem::Disconnect()
{
    client_socket_.Stop();
    Net::WSAUninit();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<SessionSubsystem>("SessionSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
