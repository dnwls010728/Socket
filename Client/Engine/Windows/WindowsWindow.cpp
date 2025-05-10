#include "pch.h"
#include "WindowsWindow.h"

#include "WindowDefinition.h"

WindowsWindow::WindowsWindow() :
    application_(nullptr),
    hWnd_(nullptr),
    definition_(nullptr)
{
}

WindowsWindow::~WindowsWindow()
{
}

WCHAR WindowsWindow::window_class[] = L"Fusion2DWindow";

std::shared_ptr<WindowsWindow> WindowsWindow::Make()
{
    return std::make_shared<WindowsWindow>();
}

void WindowsWindow::Init(WindowsApplication* const application, const std::shared_ptr<WindowDefinition>& kDefinition, HINSTANCE instance_handle, const std::shared_ptr<WindowsWindow>& kParentWindow)
{
    application_ = application;
    definition_ = kDefinition;

    uint32_t window_ex_style = 0;
    uint32_t window_style = 0;
    
    window_style |= WS_OVERLAPPED;
    window_style |= WS_CAPTION;
    window_style |= WS_SYSMENU;
    // window_style |= WS_THICKFRAME;
    window_style |= WS_MINIMIZEBOX;

    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, false, window_ex_style);

    uint32_t window_x = kDefinition->screen_x;
    uint32_t window_y = kDefinition->screen_y;
    uint32_t window_width = kDefinition->width;
    uint32_t window_height = kDefinition->height;

    window_x += border_rect.left;
    window_y += border_rect.top;
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    hWnd_ = CreateWindowEx(
        window_ex_style,
        window_class,
        kDefinition->title.c_str(),
        window_style,
        window_x, window_y,
        window_width, window_height,
        kParentWindow ? kParentWindow->GetHWnd() : nullptr,
        nullptr,
        instance_handle,
        application
        );

    if (!hWnd_) return;
    ShowWindow(hWnd_, SW_SHOW);
}

void WindowsWindow::SetWindowFocus()
{
    if (GetFocus() != hWnd_) SetFocus(hWnd_);
}

void WindowsWindow::Destroy()
{
    DestroyWindow(hWnd_);
}
