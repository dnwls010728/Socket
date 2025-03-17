#include "pch.h"
#include "Core.h"

#include "GameEngine.h"
#include "Audio/AudioManager.h"
#include "Event/EventManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Math/Vector2.h"
#include "Asset/AssetManager.h"
#include "Time/Time.h"
#include "UI_OLD/Canvas_OLD.h"
#include "Windows/WindowDefinition.h"
#include "Windows/WindowsWindow.h"
#include "Windows/DX/Renderer.h"

Core::Core() :
    game_window_(),
    game_engine_(nullptr),
    is_running_(false),
    main_thread_(),
    mutex_(),
    current_time_(0.),
    last_time_(0.),
    delta_time_(0.)
{
}

void Core::Init()
{
    WindowsApplication* application = WindowsApplication::Get();
    application->AddMessageHandler(*this);

    // DirectX 11 렌더러 초기화
    CHECK_IF(Renderer::Get()->Init(), L"Failed to initialize renderer.");

    const int screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height = GetSystemMetrics(SM_CYSCREEN);

    EngineSettings* settings = EngineSettings::Get();

    // 게임 윈도우 정의 생성
    std::shared_ptr<WindowDefinition> definition = std::make_shared<WindowDefinition>();
    definition->title = settings->GetWindowTitle();
    definition->screen_x = screen_width * .5f - settings->GetScreenWidth() * .5f;
    definition->screen_y = screen_height * .5f - settings->GetScreenHeight() * .5f;
    definition->width = settings->GetScreenWidth();
    definition->height = settings->GetScreenHeight();

    // 게임 윈도우 생성
    std::shared_ptr<WindowsWindow> new_window = application->MakeWindow();
    application->InitWindow(new_window, definition, nullptr);

    // 렌더러에 뷰포트 생성
    CHECK_IF(Renderer::Get()->CreateViewport(new_window, {definition->width, definition->height}), L"Failed to create viewport.");
    CHECK_IF(Renderer::Get()->CreateD2DViewport(new_window), L"Failed to create D2D viewport.");

    game_window_ = new_window;

    // 게임 엔진 생성
    game_engine_ = std::make_shared<GameEngine>();
    game_engine_->Init(new_window);

    current_time_ = Time::Init();

    Start();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Core::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Type::uint32 handler_result)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return true;
    if (EventManager::Get()->ProcessMessage(hWnd, message, wParam, lParam, handler_result)) return true;

    if (message == WM_SETFOCUS)
    {
        AudioManager::Get()->SetAllMutes(false);
        return true;
    }

    if (message == WM_KILLFOCUS)
    {
        EventManager::Get()->Clear();
        Keyboard::Get()->Clear();
        Mouse::Get()->Clear();
        
        AudioManager::Get()->SetAllMutes(true);
        return true;
    }

    if (message == WM_DESTROY)
    {
        if (const auto kWindow = game_window_.lock())
        {
            if (kWindow->GetHWnd() == hWnd)
            {
                Stop();

                World::Get()->Release();
                Renderer::Get()->Release();
                AssetManager::Get()->Release();
            }
        }
        return true;
    }

    return false;
}

void Core::MainThread()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!is_running_)
            {
                game_engine_->OnQuit();
                break;
            }
        }
        
#pragma region DeltaTime
        last_time_ = current_time_;
        current_time_ = Time::Seconds();

        double elapsed_time = current_time_ - last_time_;
        delta_time_ = elapsed_time;
#pragma endregion
        
        if (const auto& kWindow = game_window_.lock())
        {
            // Windows 이벤트 처리
            EventManager* event_manager = EventManager::Get();
            Event event;
            while (event_manager->PollEvent(event))
            {
                const Type::uint32& kType = event.type;
                if (kType == static_cast<Type::uint32>(EventType::kWindowSize))
                {
                    const WindowEvent& kWindowEvent = event.window;
            
                    Renderer::Get()->ResizeViewport(kWindow, kWindowEvent.data1, kWindowEvent.data2);
                }

                Keyboard::Get()->OnEvent(event);
                Mouse::Get()->OnEvent(event);
                Canvas_OLD::Get()->OnEvent(event);
                World::Get()->OnEvent(event);
            }
            
            game_engine_->GameLoop(delta_time_);
            
            Keyboard::Get()->UpdateKeyStates();
            Mouse::Get()->UpdateButtonStates();
        }
    }
}

void Core::Start()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        is_running_ = true;
    }
    
    main_thread_ = std::thread(&Core::MainThread, this);
}

void Core::Stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        is_running_ = false;
    }
    
    if (main_thread_.joinable())
    {
        main_thread_.join();
    }
}
