#include "pch.h"
#include "SessionSubsystem.h"

#include <CustomSerializer.h>

#include "NetworkManager.h"
#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"

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
