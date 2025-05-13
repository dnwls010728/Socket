#include "pch.h"
#include "SessionSubsystem.h"

#include <CustomPacket.h>
#include <CustomSerializer.h>

#include "NetworkManager.h"
#include "Inventory/InventoryManager.h"
#include "UI/Inventory.h"
#include "UI/Widget/Button.h"
#include "Windows/WindowsApplication.h"

SessionSubsystem::SessionSubsystem() :
    state_(SessionState::kNone),
    character_info_(),
    inventory_manager_(nullptr)
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

        switch (packet->GetPacketID())
        {
        case SelectCharacterResponse::StaticPacketID:
            {
                SelectCharacterResponse* response = static_cast<SelectCharacterResponse*>(packet.get());
                if (response->is_success)
                {
                    inventory_manager_ = std::make_unique<InventoryManager>();
                    inventory_manager_->AddSlot(7, 101, 20);

                    std::shared_ptr<UI::Inventory> inventory = UI::Inventory::Create(L"Inventory");
                    inventory->SetPosition({400.f, 300.f});
                    inventory->SetSize({158.f, 224.f});
                    inventory->SetPivot({0.f, 1.f});
                    inventory->AddToViewport();
                }
            }
            break;
        }
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
