#include "pch.h"
#include "SessionSubsystem.h"

#include <CustomPacket.h>
#include <CustomSerializer.h>

#include "GameInstance.h"
#include "NetworkManager.h"
#include "PacketHandlers/MoveItemHandler.h"
#include "PacketHandlers/SelectCharacterHandler.h"
#include "UI/UILoginState.h"
#include "UI/Widget/Button.h"
#include "Windows/WindowsApplication.h"

SessionSubsystem::SessionSubsystem() :
    state_(SessionState::kNone),
    handlers_()
{
}

void SessionSubsystem::Init()
{
    GameInstanceSubsystem::Init();

    client_socket_.SetSerializerFactory([]()
    {
        return std::make_unique<CustomSerializer>();
    });

    bool result = Connect({"127.0.0.1", 9000});
    // bool result = Connect({"175.198.74.36", 9000});
    if (!result)
    {
        MessageBox(nullptr, L"서버와 연결할 수 없습니다.", EngineSettings::Get()->GetWindowTitle().c_str(), MB_OK);
        WindowsApplication::Get()->QuitApplication();
        return;
    }

    // 핸들러 등록
    handlers_.emplace(
        SelectCharacterResponse::StaticPacketID,
        std::make_unique<SelectCharacterHandler>()
    );

    handlers_.emplace(
        MoveItemResponse::StaticPacketID,
        std::make_unique<MoveItemHandler>()
    );

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
                WindowsApplication::Get()->QuitApplication();
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
    DisconnectPacket disconnect_packet;
    client_socket_.SendPacket(disconnect_packet);
    
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
