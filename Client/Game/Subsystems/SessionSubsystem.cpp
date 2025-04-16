#include "pch.h"
#include "SessionSubsystem.h"

#include <CustomSerializer.h>

#include "NetworkManager.h"
#include "Windows/WindowsApplication.h"

SessionSubsystem::SessionSubsystem()
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
    });
}

void SessionSubsystem::SendPacket(Net::IPacket& packet)
{
    client_socket_.SendPacket(packet);
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
