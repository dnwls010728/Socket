#include "pch.h"
#define _CRTDBG_MAP_ALLOC

#include "Client.h"
#include "Core.h"

#include <crtdbg.h>
#include <iostream>

#include "resource.h"
#include "SocketCore/ServerPacketHandler.h"
#include "Windows/WindowsWindow.h"
#include "SocketCore/SocketSession.h"
#include "SocketCore/Packet.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    
    HICON icon_handle = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    WindowsApplication* application = new WindowsApplication(hInstance, icon_handle);

    Core* core = new Core();
    core->Init(application);

    //임시 소켓 테스팅
    if(GSocketSession->Connect())
    {
        C_EnterPacket pkt;
        pkt.SetId("Client");
        pkt.SetName("Client");
        auto sendBuffer = ServerPacketHandler::MakeSendBuffer<C_EnterPacket>(pkt,C_PKT_ENTER);
        GSocketSession->Send(sendBuffer);
    };
    
    application->PumpMessages();

    SAFE_RELEASE(core);
    SAFE_RELEASE(application);
    
    return 0;
}
