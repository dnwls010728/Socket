#pragma once
#include "Level/World.h"
#include "Windows/WindowsApplication.h"

class Renderer;
class GameEngine;

class Core : public IWindowsMessageHandler
{
public:
    Core();
    ~Core() = default;

    void Init();
    
    virtual bool ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, uint32_t handler_result) override;

private:
    void MainThread();
    void Start();
    void Stop();
    
    std::weak_ptr<WindowsWindow> game_window_;

    std::shared_ptr<GameEngine> game_engine_;
    
    std::atomic<bool> is_running_;
    std::thread main_thread_;
    std::mutex mutex_;

    double current_time_;
    double last_time_;
    double delta_time_;
    
};
