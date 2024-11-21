#include "pch.h"
#define _CRTDBG_MAP_ALLOC

#include "Client.h"
#include "Engine/Core.h"

#include <crtdbg.h>
#include <iostream>

#include "resource.h"
#include "SocketCore/ServerPacketHandler.h"
#include "Engine/Windows/WindowsWindow.h"
#include "SocketCore/SocketSession.h"
#include "../Common/Packet.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    
    HICON icon_handle = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    
    WindowsApplication* application = WindowsApplication::Get();
    application->Init(hInstance, icon_handle);

    std::unique_ptr<Core> core = std::make_unique<Core>();
    core->Init();

    ServerPacketHandler::Init();
    
    application->PumpMessages();
    
    return 0;
}
