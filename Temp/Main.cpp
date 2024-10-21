#include "Main.h"
#include <Windows.h>

#include "Windows/WindowDefinition.h"
#include "Windows/WindowsApplication.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    WindowsApplication* application = new WindowsApplication(hInstance, nullptr);
    
    const int screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height = GetSystemMetrics(SM_CYSCREEN);

    // 게임 윈도우 정의 생성
    std::shared_ptr<WindowDefinition> definition = std::make_shared<WindowDefinition>();
    definition->title = L"";
    definition->screen_x = screen_width * .5f - 640.f * .5f;
    definition->screen_y = screen_height * .5f - 480.f * .5f;
    definition->width = 640.f;
    definition->height = 480.f;

    // 게임 윈도우 생성
    std::shared_ptr<WindowsWindow> new_window = application->MakeWindow();
    application->InitWindow(new_window, definition, nullptr);

    
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    delete application;
    return 0;
}
