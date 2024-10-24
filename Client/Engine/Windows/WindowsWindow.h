#pragma once

#include "../framework.h"
#include "Misc/EngineMacros.h"

struct WindowDefinition;
class WindowsApplication;

class WindowsWindow
{
public:
    WindowsWindow();
    ~WindowsWindow();

    static WCHAR window_class[];

    static std::shared_ptr<WindowsWindow> Make();

    void Init(WindowsApplication* const application, const std::shared_ptr<WindowDefinition>& kDefinition, HINSTANCE instance_handle, const std::shared_ptr<WindowsWindow>& kParentWindow);
    void SetWindowFocus();
    void Destroy();

    FORCEINLINE HWND GetHWnd() const { return hWnd_; }
    FORCEINLINE const WindowDefinition* GetDefinition() const { return definition_.get(); }

private:
    WindowsApplication* application_;
    HWND hWnd_;

    std::shared_ptr<WindowDefinition> definition_;
    
};