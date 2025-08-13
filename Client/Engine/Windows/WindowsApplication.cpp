#include "pch.h"
#include "WindowsApplication.h"

#include "WindowsWindow.h"
#include "combaseapi.h"
#include "Event/EventManager.h"

WindowsApplication::WindowsApplication() :
    instance_handle_(nullptr),
    windows_()
{
}

WindowsApplication::~WindowsApplication()
{
    CoUninitialize();
}

ATOM WindowsApplication::RegisterClass(const HINSTANCE instance_handle, const HICON icon_handle)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = StaticWndProc;
    wcex.hInstance = instance_handle;
    wcex.hIcon = icon_handle;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszClassName = WindowsWindow::window_class;

    return RegisterClassEx(&wcex);
}

std::shared_ptr<WindowsWindow> WindowsApplication::MakeWindow()
{
    return WindowsWindow::Make();
}

void WindowsApplication::Init(const HINSTANCE instance_handle, const HICON icon_handle)
{
    instance_handle_ = instance_handle;
    
    RegisterClass(instance_handle, icon_handle);
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
}

void WindowsApplication::InitWindow(const std::shared_ptr<WindowsWindow>& kWindow, const std::shared_ptr<WindowDefinition>& kDefinition, const std::shared_ptr<WindowsWindow>& kParentWindow)
{
    windows_.push_back(kWindow);
    kWindow->Init(this, kDefinition, instance_handle_, kParentWindow);
}

void WindowsApplication::AddMessageHandler(IWindowsMessageHandler& message_handler)
{
    for (const auto& handler : message_handlers_)
    {
        if (handler == &message_handler) return;
    }
    
    message_handlers_.push_back(&message_handler);
}

void WindowsApplication::RemoveMessageHandler(IWindowsMessageHandler& message_handler)
{
    std::erase(message_handlers_, &message_handler);
}

void WindowsApplication::PumpMessages()
{
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (GetMessage(&msg, nullptr, 0, 0))
        {
            EventManager::Get()->SetMessageTick(msg.time);
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void WindowsApplication::QuitApplication()
{
    for (const auto& window : windows_)
    {
        PostMessage(window->GetHWnd(), WM_USER, 0, 0);
    }
}

void WindowsApplication::EnableCursor()
{
    ShowCursor();
}

void WindowsApplication::DisableCursor()
{
    HideCursor();
}

LRESULT WindowsApplication::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WindowsApplication* application;

    if (message == WM_NCCREATE)
    {
        LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        application = static_cast<WindowsApplication*>(create_struct->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
    }
    else application = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (application) return application->ProcessMessage(hWnd, message, wParam, lParam);
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowsApplication::ShowCursor()
{
    while (::ShowCursor(TRUE) < 0);
}

void WindowsApplication::HideCursor()
{
    while (::ShowCursor(FALSE) >= 0);
}

uint32_t WindowsApplication::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    std::shared_ptr<WindowsWindow> window = FindWindowByHWND(hWnd);
    
    if (window)
    {
        bool is_external_handled = false;
        uint32_t external_handler_result = 0;
        
        for (const auto& handler : message_handlers_)
        {
            uint32_t handler_result = 0;
            if (handler->ProcessMessage(hWnd, message, wParam, lParam, handler_result))
            {
                if (!is_external_handled)
                {
                    is_external_handled = true;
                    external_handler_result = handler_result;
                }
            }
        }
        
        if (message == WM_GETMINMAXINFO)
        {
            uint32_t window_ex_style = 0;
            uint32_t window_style = 0;
    
            window_style |= WS_OVERLAPPED;
            window_style |= WS_CAPTION;
            window_style |= WS_SYSMENU;
            window_style |= WS_THICKFRAME;
            window_style |= WS_MINIMIZEBOX;

            RECT border_rect = {0, 0, 0, 0};
            AdjustWindowRectEx(&border_rect, window_style, false, window_ex_style);
        
            MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 100 + border_rect.right - border_rect.left;
            info->ptMinTrackSize.y = 100 + border_rect.bottom - border_rect.top;

            return 0;
        }

        if (message == WM_ACTIVATE)
        {
            if (wParam & WA_ACTIVE || wParam & WA_CLICKACTIVE) HideCursor();
            else ShowCursor();
        }
        
        if (message == WM_DESTROY)
        {
            std::erase(windows_, window);

            if (windows_.empty())
            {
                PostQuitMessage(0);
            }
            return 0;
        }

        if (message == WM_USER)
        {
            window->Destroy();
            return 0;
        }

        if (is_external_handled) return external_handler_result;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

std::shared_ptr<WindowsWindow> WindowsApplication::FindWindowByHWND(HWND hWnd) const
{
    for (const auto& window : windows_)
    {
        if (window->GetHWnd() == hWnd) return window;
    }
    
    return nullptr;
}