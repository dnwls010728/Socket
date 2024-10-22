#include "pch.h"
#include "EngineModule.h"

#include "../Client/Engine/Misc/EngineMacros.h"
#include "Windows/WindowsApplication.h"

EngineModule::EngineModule()
{
}

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
    )
{
    WindowsApplication* app = new WindowsApplication(hInstance, nullptr);
    app->PumpMessages();
    
    SAFE_RELEASE(app);
    return 0;
}
